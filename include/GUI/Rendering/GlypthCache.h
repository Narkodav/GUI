#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftcolor.h>
#include <freetype/tttables.h>

#include <hb.h>          // Core functionality
#include <hb-ft.h>       // FreeType integration
#include <hb-ot.h>

#include <unicode/unistr.h>
#include <unicode/uscript.h>
#include <unicode/ubidi.h>

#include <string>
#include <string_view>
#include <stdexcept>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GUI/Instance.h"
#include "GUI/Rendering/Glypth.h"

namespace GUI
{
    // separate loader for each font and size pair
    class GlypthCache {
    private:
        FT_Face m_face;
        FontType m_type;
        size_t m_size;

        mutable std::unordered_map<CharId, Glypth> m_glypthMap;

        using GlypthCacheFunc = Glypth (GlypthCache::*)(GUI::InstanceInterface& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
            const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
            Graphics::PhysicalDevice physicalDevice, CharId character) const;
        const std::array<GlypthCacheFunc, static_cast<size_t>(FontType::Count)> m_getGlypthTable = {
            &GlypthCache::getGlypthGrayscale,   //Normal                                     // tested
            &GlypthCache::getGlypthGrayscale,   //Colored unknown type default to greyscale  // tested
            &GlypthCache::getGlypthColrCpal,    //ColoredColrCpal                            // tested
            &GlypthCache::getGlypthCbdtCblc,    //ColoredCbdtCblc                            // not tested, should work
            &GlypthCache::getGlypthSibix        //ColoredSbix                                // not tested, should work
        };

    public:
        GlypthCache() = default;
        ~GlypthCache() = default;

        GlypthCache(const GlypthCache&) = delete;
        GlypthCache& operator=(const GlypthCache&) = delete;

        GlypthCache(GlypthCache&& other) = default;
        GlypthCache& operator=(GlypthCache&& other) = default;

        void create(GUI::InstanceInterface& instance, std::string_view path, size_t size);
        void destroy(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device);

        inline const Glypth& getGlypth(GUI::InstanceInterface& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
            const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
            Graphics::PhysicalDevice physicalDevice, CharId character) const;

    private:
        Glypth getGlypthGrayscale(GUI::InstanceInterface& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
            const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
            Graphics::PhysicalDevice physicalDevice, CharId character) const;
        Glypth getGlypthSibix(GUI::InstanceInterface& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
            const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
            Graphics::PhysicalDevice physicalDevice, CharId character) const;
        Glypth getGlypthCbdtCblc(GUI::InstanceInterface& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
            const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
            Graphics::PhysicalDevice physicalDevice, CharId character) const;
        Glypth getGlypthColrCpal(GUI::InstanceInterface& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
            const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
            Graphics::PhysicalDevice physicalDevice, CharId character) const;

        bool glyphHasColor(CharId character) const;

        FontType getFontType() const;
    };

}
