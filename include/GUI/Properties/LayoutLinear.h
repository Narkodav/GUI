#pragma once
#include "GUI/Properties/Layout.h"
#include "GUI/Properties/SizeConstraints.h"
#include "GUI/Elements/Element.h"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace GUI
{
    enum class LayoutAxis {
        Horizontal,
        Vertical
    };

    template<LayoutAxis Axis>
    struct AxisTraits;

    template<>
    struct AxisTraits<LayoutAxis::Vertical>
    {
        static constexpr int s_main = 1;   // y
        static constexpr int s_cross = 0;  // x
    };

    template<>
    struct AxisTraits<LayoutAxis::Horizontal>
    {
        static constexpr int s_main = 0;   // x
        static constexpr int s_cross = 1;  // y
    };

    template<LayoutAxis s_axis>
	class LayoutLinear : public Layout {
		GUI_DECLARE_OBJECT(LayoutLinear<s_axis>, Layout)
    protected:
        static constexpr int s_main  = AxisTraits<s_axis>::s_main;
        static constexpr int s_cross = AxisTraits<s_axis>::s_cross;
        
	public:
        virtual ~LayoutLinear() = default;

        using Layout::Layout;

        // Calculates the minimum required size and maximum "filled" size, 
        // below min elements will be clipped and above min there will be gaps between elements
        virtual MeasureResult measureFull() override {
            MeasureResult result{{}, { glm::ivec2(0, 0), glm::ivec2(0, 0) } };
            auto* parent = getParent();

            for(size_t i = 0; i < parent->getChildCount(); ++i) {
                auto* child = parent->getChild(i);
                auto* layout = child->template getProperty<Layout>();
                auto* sizeConstraints = child->template getProperty<SizeConstraints>();
                Constraints constraint;
                if(layout != nullptr && sizeConstraints != nullptr) {
                    constraint.minSize = layout->measure();
                    constraint.minSize[s_main] = std::max(sizeConstraints->getMinSize()[s_main], constraint.minSize[s_main]);
                    constraint.minSize[s_cross] = std::max(sizeConstraints->getMinSize()[s_cross], constraint.minSize[s_cross]);
                    constraint.maxSize = sizeConstraints->getMaxSize();
                }
                else if(sizeConstraints != nullptr) {
                    constraint = sizeConstraints->getConstraints();
                }
                else if(layout != nullptr) {
                    constraint.minSize = layout->measure();
                    constraint.maxSize = glm::ivec2(s_maxElementSize, s_maxElementSize);
                }

                result.childConstraints.push_back(constraint);
                
                result.totalConstraints.minSize[s_main] += constraint.minSize[s_main];
                result.totalConstraints.maxSize[s_main] += constraint.maxSize[s_main];

                result.totalConstraints.minSize[s_cross] = std::max(result.totalConstraints.minSize[s_cross], constraint.minSize[s_cross]);
                result.totalConstraints.maxSize[s_cross] = std::max(result.totalConstraints.maxSize[s_cross], constraint.maxSize[s_cross]);
            }
            result.totalConstraints.minSize[s_main] = std::clamp(result.totalConstraints.minSize[s_main], 0, s_maxElementSize);
            result.totalConstraints.maxSize[s_main] = std::clamp(result.totalConstraints.maxSize[s_main], 0, s_maxElementSize);
            if(result.totalConstraints.minSize[s_cross] > result.totalConstraints.maxSize[s_cross]) result.totalConstraints.maxSize[s_cross] = result.totalConstraints.minSize[s_cross];
            if(result.totalConstraints.minSize[s_main] > result.totalConstraints.maxSize[s_main]) result.totalConstraints.maxSize[s_main] = result.totalConstraints.minSize[s_main];
            return result;
        }

        //Measures only min because max is defined by SizeConstraints or is s_maxElementSize
        virtual glm::ivec2 measure() override {
            glm::ivec2 result = glm::ivec2(0, 0);
            auto* parent = getParent();

            for(size_t i = 0; i < parent->getChildCount(); ++i) {
                auto* child = parent->getChild(i);
                auto* layout = child->template getProperty<Layout>();
                auto* sizeConstraints = child->template getProperty<SizeConstraints>();
                glm::ivec2 constraint;
                if(layout != nullptr && sizeConstraints != nullptr) {
                    constraint = layout->measure();

                    constraint[s_main] = std::max(sizeConstraints->getMinSize()[s_main], constraint[s_main]);
                    constraint[s_cross] = std::max(sizeConstraints->getMinSize()[s_cross], constraint[s_cross]);
                }
                else if(sizeConstraints != nullptr) {
                    constraint = sizeConstraints->getMinSize();
                }
                else if(layout != nullptr) {
                    constraint = layout->measure();
                }
                
                result[s_main] += constraint[s_main];
                result[s_cross] = std::max(result[s_cross], constraint[s_cross]);
            }
            result[s_main] = std::clamp(result[s_main], 0, s_maxElementSize);
            return result;
        }

        // Returns the difference between parent size and minimal required size, if it is positive the widget cannot fit all children
        virtual glm::ivec2 arrange() override {
            auto minMax = measureFull();
            auto* parent = getParent();
            auto parentSize = parent->getSize();
            auto& childCons = minMax.childConstraints;

            //---------------------------------------
            // Case 1: Not enough room
            //---------------------------------------
            if(minMax.totalConstraints.minSize[s_main] >= parentSize[s_main]) {
                glm::ivec2 pos(0, 0);
                glm::ivec2 size;

                for (size_t i = 0; i < childCons.size(); ++i) {
                    auto* child = parent->getChild(i);
                    
                    size[s_cross] = std::clamp(parentSize[s_cross], childCons[i].minSize[s_cross], childCons[i].maxSize[s_cross]);
                    size[s_main] = childCons[i].minSize[s_main];
                    
                    pos[s_cross] = (parentSize[s_cross] - size[s_cross]) / 2;

                    child->setSize(size);
                    child->setPosition(pos);
                    auto* layout = child->template getProperty<Layout>();
                    if(layout != nullptr) layout->arrange();
                    pos[s_main] += size[s_main];
                }
            }
            //---------------------------------------
            // Case 2: Excess room
            //---------------------------------------
            else if(minMax.totalConstraints.maxSize[s_main] < parentSize[s_main]) {
                size_t step = (parentSize[s_main] - minMax.totalConstraints.maxSize[s_main]) / (childCons.size() + 1);
                glm::ivec2 pos(0, 0);
                glm::ivec2 size;
                pos[s_main] = step;
                for (size_t i = 0; i < childCons.size(); ++i) {
                    auto* child = parent->getChild(i);
                    size[s_cross] = std::clamp(parentSize[s_cross], childCons[i].minSize[s_cross], childCons[i].maxSize[s_cross]);
                    size[s_main] = childCons[i].maxSize[s_main];

                    pos[s_cross] = (parentSize[s_cross] - size[s_cross]) / 2;
                    
                    child->setSize(size);
                    child->setPosition(pos);
                    auto* layout = child->template getProperty<Layout>();
                    if(layout != nullptr) layout->arrange();
                    pos[s_main] += childCons[i].maxSize[s_main] + step;
                }
            }
            //---------------------------------------
            // Case 3: fill
            //---------------------------------------
            else {
                std::vector<size_t> indices(childCons.size());
                for(size_t i = 0; i < childCons.size(); ++i) indices[i] = i;
                std::sort(indices.begin(), indices.end(), [&childCons](size_t lhs, size_t rhs){
                    return childCons[lhs].maxSize[s_main] - childCons[lhs].minSize[s_main] < childCons[rhs].maxSize[s_main] - childCons[rhs].minSize[s_main];
                });

                glm::ivec2 size;
                size_t remaining = parentSize[s_main] - minMax.totalConstraints.minSize[s_main];
                size_t share;
                for(size_t i = 0; i < indices.size(); ++i){
                    share = remaining / (childCons.size() - i);
                    auto& childCon = childCons[indices[i]];
                    if(childCon.maxSize[s_main] - childCon.minSize[s_main] < share) {
                        auto* child = parent->getChild(indices[i]);
                        size[s_cross] = std::clamp(parentSize[s_cross], childCon.minSize[s_cross], childCon.maxSize[s_cross]);
                        size[s_main] = childCon.maxSize[s_main];
                        child->setSize(size);
                        remaining -= childCon.maxSize[s_main] - childCon.minSize[s_main];
                    }
                    else {
                        for(; i < indices.size(); ++i){
                            auto& childCon = childCons[indices[i]];
                            auto* child = parent->getChild(indices[i]);
                            size[s_cross] = std::clamp(parentSize[s_cross], childCon.minSize[s_cross], childCon.maxSize[s_cross]);
                            size[s_main] = childCon.minSize[s_main] + share;
                            child->setSize(size);
                        }
                        break;
                    }
                }
                // Last pass to position
                glm::ivec2 pos(0, 0);
                for(size_t i = 0; i < childCons.size(); ++i){
                    auto* child = parent->getChild(i);
                    auto size = child->getSize();
                    pos[s_cross] = (parentSize[s_cross] - size[s_cross]) / 2;
                    child->setPosition(pos);
                    auto* layout = child->template getProperty<Layout>();
                    if(layout != nullptr) layout->arrange();
                    pos[s_main] += size[s_main];
                }
            }
            return minMax.totalConstraints.minSize - parentSize;
        }
	};
}