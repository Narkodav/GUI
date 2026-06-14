#pragma once
#include "GUI/Rendering/Glyph.h"

namespace GUI
{
    class Text;
    // separate loader for each font and size pair
    class Font {
    public:
        class Size {
            friend class Font;
            friend class Text;
        public:

            // In pixels
            struct Metrics {
                int32_t ascender;
                int32_t descender;
                int32_t height;
                int32_t maxAdvance;
            };

        private:
            Font* m_font;
            FT_Size m_size;
            Metrics m_metrics;
            mutable std::unordered_map<CharId, Glyph> m_glyphMap;

            int32_t create(Font* font, size_t size) {
                m_font = font;
                FT_New_Size(m_font->m_face, &m_size);
                FT_Activate_Size(m_size);
                FT_Set_Pixel_Sizes(m_font->m_face, 0, size);

                m_metrics.ascender = getFace()->size->metrics.ascender >> 6;
                m_metrics.descender = -(getFace()->size->metrics.descender >> 6);
                m_metrics.height = getFace()->size->metrics.height >> 6;
                m_metrics.maxAdvance = getFace()->size->metrics.max_advance >> 6;

                return m_metrics.height;
            }

            void destroy(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device) {
                FT_Done_Size(m_size);
                for(auto& glyph : m_glyphMap) {
                    glyph.second.destroy(functions, device);
                }
                m_glyphMap.clear();
            }

        public:

            Size() = default;
            ~Size() = default;

            Size(const Size&) = delete;
            Size& operator=(const Size&) = delete;

            Size(Size&& other) = default;
            Size& operator=(Size&& other) = default;

            const Glyph& getGlyph(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
                const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
                Graphics::PhysicalDevice physicalDevice, CharId character) const;

            const Metrics& getMetrics() const { return m_metrics; }

        private:
            Glyph getGlyphGrayscale(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
                const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
                Graphics::PhysicalDevice physicalDevice, CharId character) const;
            Glyph getGlyphSibix(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
                const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
                Graphics::PhysicalDevice physicalDevice, CharId character) const;
            Glyph getGlyphCbdtCblc(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
                const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
                Graphics::PhysicalDevice physicalDevice, CharId character) const;
            Glyph getGlyphColrCpal(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
                const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
                Graphics::PhysicalDevice physicalDevice, CharId character) const;

            bool glyphHasColor(CharId character) const;

            FT_Face getFace() const { return m_font->m_face; }
        };

        friend class Size;

    private:
        FT_Face m_face;
        FontType m_type;
        mutable std::unordered_map<size_t, Size> m_sizes;

    public:
        Font() = default;
        ~Font() = default;

        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

        Font(Font&& other) = default;
        Font& operator=(Font&& other) = default;

        void create(GUI::InstanceInterface& instance, std::string_view path);
        void destroy(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device);

        const Size& getSize(size_t height) {
            auto it = m_sizes.find(height);
            if (it != m_sizes.end()) { return it->second; }
            
            Size newSize;
            height = newSize.create(this, height);

            return m_sizes.emplace(height, std::move(newSize)).first->second;
        }

    private:

        FontType getFontType() const;
    };

}
