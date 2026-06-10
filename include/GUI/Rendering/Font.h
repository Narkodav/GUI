#pragma once
#include "GUI/Rendering/Glypth.h"

namespace GUI
{
    class Text;
    // separate loader for each font and size pair
    class Font {
    public:
        class Size {
            friend class Font;
            friend class Text;
        private:
            Font* m_font;
            FT_Size m_size;
            mutable std::unordered_map<CharId, Glypth> m_glypthMap;

            Size(Font* font, size_t size) : m_font(font)  {
                FT_New_Size(m_font->m_face, &m_size);
                FT_Activate_Size(m_size);
                FT_Set_Pixel_Sizes(m_font->m_face, 0, size);
            }

            void destroy(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device) {
                FT_Done_Size(m_size);
                for(auto& glypth : m_glypthMap) {
                    glypth.second.destroy(functions, device);
                }
                m_glypthMap.clear();
            }

        public:

            Size() = default;
            ~Size() = default;

            Size(const Size&) = delete;
            Size& operator=(const Size&) = delete;

            Size(Size&& other) = default;
            Size& operator=(Size&& other) = default;

            const Glypth& getGlypth(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
                const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
                Graphics::PhysicalDevice physicalDevice, CharId character) const;

        private:
            Glypth getGlypthGrayscale(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
                const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
                Graphics::PhysicalDevice physicalDevice, CharId character) const;
            Glypth getGlypthSibix(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
                const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
                Graphics::PhysicalDevice physicalDevice, CharId character) const;
            Glypth getGlypthCbdtCblc(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
                const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
                Graphics::PhysicalDevice physicalDevice, CharId character) const;
            Glypth getGlypthColrCpal(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
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

        const Size& getSize(size_t size) {
            auto it = m_sizes.find(size);
            if (it != m_sizes.end()) { return it->second; }
            
            Size newSize(this, size);

            return m_sizes.emplace(size, std::move(newSize)).first->second;
        }

    private:

        FontType getFontType() const;
    };

}
