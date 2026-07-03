#pragma once
#include "GUI/Elements/Element.h"
#include "GUI/Properties/Property.h"

namespace GUI::Utility {
    template<PropertyConcept T>
    class PropertyDispatch {
    private:
        using TargetPair = std::pair<Object::Subscription, T*>;
        std::vector<std::pair<Object::Subscription, T*>> m_targets;
    public:
        template<PropertyConcept U>
        void registerTarget(U* target) requires std::derived_from<U, T> {
            TargetPair pair = { target->getParent()->Object::template subscribe<ObjectEvent::ObjectDestroyed>(
                [this, target](GUI::Object*){ unregisterTarget(target); }), target };
            m_targets.push_back(std::move(pair));
        }

        template<PropertyConcept U>
        void unregisterTarget(U* target) requires std::derived_from<U, T> {
            auto it = std::find_if(m_targets.begin(), m_targets.end(),
                [target](const auto& t) { return t.second == target; });
            if (it != m_targets.end()) {
                it->second->getParent()->Object::template unsubscribe<ObjectEvent::ObjectDestroyed>(it->first);
                m_targets.erase(it);
            }
        }

        template<auto Fn, typename... Args>
        void dispatch(Args&&... args) {
            for(auto& target : m_targets) {
                (target.second->*Fn)(std::forward<Args>(args)...);
            }
        }

        template <typename Fn, typename... Args>
        void dispatch(Fn fn, Args&&... args) {
            for(auto& target : m_targets) {
                std::invoke(fn, target.second, std::forward<Args>(args)...);
            }
        }
    };
}