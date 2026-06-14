#include "GUI/Rendering/Font.h"
#include "GUI/Instance.h"

namespace GUI {
    void Font::create(GUI::InstanceInterface& instance, std::string_view path) {
        if (FT_New_Face(instance.getFTInstance(), path.data(), 0, &m_face))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

        if (FT_Select_Charmap(m_face, FT_ENCODING_UNICODE)) {
            throw std::runtime_error("ERROR::FREETYPE: Failed to set Unicode charmap");
        }

        m_sizes.clear();
        m_type = getFontType();
    }

    void Font::destroy(const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device) {
        for(auto& size : m_sizes) {
            size.second.destroy(functions, device);
        }
        FT_Done_Face(m_face);
        m_sizes.clear();
    }

    FontType Font::getFontType() const {
        if ((m_face->face_flags & FT_FACE_FLAG_COLOR) != 0) {
            // Check for COLR/CPAL
            FT_ULong length = 0;
            bool hasColr = !FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('C', 'O', 'L', 'R'), 0, NULL, &length) && length > 0;
            length = 0;
            bool hasCpal = !FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('C', 'P', 'A', 'L'), 0, NULL, &length) && length > 0;

            if (hasColr && hasCpal) return FontType::ColoredColrCpal;

            // Check for CBDT/CBLC
            length = 0;
            bool hasCbdt = !FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('C', 'B', 'D', 'T'), 0, NULL, &length) && length > 0;
            length = 0;
            bool hasCblc = !FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('C', 'B', 'L', 'C'), 0, NULL, &length) && length > 0;

            if (hasCbdt && hasCblc) return FontType::ColoredCbdtCblc;

            // Check for SBIX
            length = 0;
            if (!FT_Load_Sfnt_Table(m_face, FT_MAKE_TAG('s', 'b', 'i', 'x'), 0, NULL, &length) && length > 0) {
                return FontType::ColoredSbix;
            }

            // Color font but unknown type
            return FontType::Colored;
        }
        else return FontType::Normal;
        return FontType::Colored; //couldn't determine colored type
    }

    bool Font::Size::glyphHasColor(CharId character) const {
        FT_LayerIterator iterator;
        iterator.p = nullptr;
        FT_UInt layerGlyphIndex;
        FT_UInt layerColorIndex;

        return FT_Get_Color_Glyph_Layer(m_font->m_face, character, &layerGlyphIndex, &layerColorIndex, &iterator);
    }

    Glyph Font::Size::getGlyphGrayscale(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
        const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
        Graphics::PhysicalDevice physicalDevice, CharId character) const {
        Glyph glyph;
        if (FT_Load_Glyph(m_font->m_face, character, FT_LOAD_RENDER))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        auto& glyphSlot = m_font->m_face->glyph;

        glyph.create(instance, instanceFunctions, functions, device, physicalDevice, {
                glm::ivec2(glyphSlot->bitmap.width, glyphSlot->bitmap.rows),
                glm::ivec2(glyphSlot->bitmap_left, glyphSlot->bitmap_top),
                glm::ivec2(glyphSlot->advance.x, glyphSlot->advance.y)
            }, character,
            glyphSlot->bitmap.buffer,
            glyphSlot->bitmap.width,
            glyphSlot->bitmap.rows,
            glyphSlot->bitmap.pitch,
            1,
            Graphics::Format::R8Unorm
        );
        return glyph;
    }

    Glyph Font::Size::getGlyphSibix(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
        const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
        Graphics::PhysicalDevice physicalDevice, CharId character) const {
        Glyph glyph;
        if (FT_Load_Glyph(m_font->m_face, character, FT_LOAD_COLOR | FT_LOAD_RENDER))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        auto& glyphSlot = m_font->m_face->glyph;

        glyph.create(instance, instanceFunctions, functions, device, physicalDevice, {
                glm::ivec2(glyphSlot->bitmap.width, glyphSlot->bitmap.rows),
                glm::ivec2(glyphSlot->bitmap_left, glyphSlot->bitmap_top),
                glm::ivec2(glyphSlot->advance.x, glyphSlot->advance.y)
            }, character,
            glyphSlot->bitmap.buffer,
            glyphSlot->bitmap.width,
            glyphSlot->bitmap.rows,
            glyphSlot->bitmap.pitch,
            4,
            Graphics::Format::R8G8B8A8Unorm
        );
        return glyph;
    }

    Glyph Font::Size::getGlyphCbdtCblc(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
        const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
        Graphics::PhysicalDevice physicalDevice, CharId character) const {
        Glyph glyph;
        if (FT_Load_Glyph(m_font->m_face, character, FT_LOAD_COLOR | FT_LOAD_RENDER))
            throw std::runtime_error("ERROR::FREETYPE: Failed to load a character");

        auto& glyphSlot = m_font->m_face->glyph;

        glyph.create(instance, instanceFunctions, functions, device, physicalDevice, {
                glm::ivec2(glyphSlot->bitmap.width, glyphSlot->bitmap.rows),
                glm::ivec2(glyphSlot->bitmap_left, glyphSlot->bitmap_top),
                glm::ivec2(glyphSlot->advance.x, glyphSlot->advance.y)
            }, character,
            glyphSlot->bitmap.buffer,
            glyphSlot->bitmap.width,
            glyphSlot->bitmap.rows,
            glyphSlot->bitmap.pitch,
            4,
            Graphics::Format::R8G8B8A8Unorm
        );
        return glyph;
    }

    Glyph Font::Size::getGlyphColrCpal(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
        const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
        Graphics::PhysicalDevice physicalDevice, CharId character) const {

        auto& face = m_font->m_face;

        Glyph glyph;
        if (!glyphHasColor(character))
            return getGlyphGrayscale(instance, instanceFunctions, functions, device, physicalDevice, character);

        // First pass: calculate total bounding box
        //FT_BBox bbox;
        //bbox.xMin = bbox.yMin = 32000;
        //bbox.xMax = bbox.yMax = -32000;

        FT_LayerIterator iterator;
        iterator.p = nullptr;
        FT_UInt layerGlyphIndex;
        FT_UInt layerColorIndex;

        // Get initial metrics
        FT_Load_Glyph(face, character, FT_LOAD_DEFAULT);
        int originalLeft = face->glyph->bitmap_left;
        int originalTop = face->glyph->bitmap_top;

        //// First pass to get total bounds
        //while (FT_Get_Color_Glyph_Layer(m_face, character, &layerGlyphIndex, &layerColorIndex, &iterator)) {
        //    FT_Load_Glyph(m_face, layerGlyphIndex, FT_LOAD_DEFAULT);
        //    FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);

        //    FT_Bitmap* bitmap = &m_face->glyph->bitmap;

        //    // Update bounding box
        //    int x1 = m_face->glyph->bitmap_left;
        //    int y1 = m_face->glyph->bitmap_top - bitmap->rows;
        //    int x2 = x1 + bitmap->width;
        //    int y2 = m_face->glyph->bitmap_top;

        //    bbox.xMin = std::min<FT_Pos>(bbox.xMin, x1);
        //    bbox.yMin = std::min<FT_Pos>(bbox.yMin, y1);
        //    bbox.xMax = std::max<FT_Pos>(bbox.xMax, x2);
        //    bbox.yMax = std::max<FT_Pos>(bbox.yMax, y2);
        //}

        //// Calculate dimensions from bounding box
        //int width = bbox.xMax - bbox.xMin;
        //int height = bbox.yMax - bbox.yMin;

        //// Ensure minimum dimensions
        //width = std::max(1, width);
        //height = std::max(1, height);

        int width = face->glyph->bitmap.width;
        int height = face->glyph->bitmap.rows;

        if (width == 0 || height == 0) {
            glyph.create(instance, instanceFunctions, functions, device, physicalDevice, {
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    glm::ivec2(face->glyph->advance.x, face->glyph->advance.y)
                }, character,
                face->glyph->bitmap.buffer,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                face->glyph->bitmap.pitch,
                4,
                Graphics::Format::R8G8B8A8Unorm
            );
            return glyph;
        }

        // Create bitmap buffer for BGRA
        std::vector<uint8_t> bitmapVector(width * height * 4, 0);

        // Get color layer information
        iterator.p = nullptr;

        // Get palette information
        FT_Color* palette = nullptr;
        FT_Error error = FT_Palette_Select(face, 0, &palette);
        if (error || !palette) {
            // Handle error or no palette case
            glyph.create(instance, instanceFunctions, functions, device, physicalDevice, {
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    glm::ivec2(face->glyph->advance.x, face->glyph->advance.y)
                }, character,
                face->glyph->bitmap.buffer,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                face->glyph->bitmap.pitch,
                4,
                Graphics::Format::R8G8B8A8Unorm
            );
            return glyph;
        }

        // Iterate through all layers
        while (FT_Get_Color_Glyph_Layer(face, character, &layerGlyphIndex, &layerColorIndex, &iterator)) {
            // Load and render layer
            FT_Load_Glyph(face, layerGlyphIndex, FT_LOAD_DEFAULT);
            FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

            FT_Bitmap* bitmap = &face->glyph->bitmap;
            FT_Color color = palette[layerColorIndex];

            // Calculate offsets for proper positioning
            int startX = face->glyph->bitmap_left - originalLeft;
            int startY = originalTop - face->glyph->bitmap_top;

            // Blend layer
            for (unsigned int y = 0; y < bitmap->rows; y++) {
                for (unsigned int x = 0; x < bitmap->width; x++) {
                    // Calculate destination position
                    int destX = startX + x;
                    int destY = startY + y;

                    // Check bounds
                    if (destX >= 0 && destX < width &&
                        destY >= 0 && destY < height) {

                        unsigned char coverage = bitmap->buffer[y * bitmap->pitch + x];
                        int idx = (destY * width + destX) * 4;

                        // Alpha blending with existing content
                        float src_a = coverage / 255.0f;
                        float dst_a = bitmapVector[idx + 3] / 255.0f;
                        float out_a = src_a + dst_a * (1.0f - src_a);

                        if (out_a > 0.0f) {
                            // Premultiplied alpha blending
                            float src_weight = src_a / out_a;
                            float dst_weight = (dst_a * (1.0f - src_a)) / out_a;

                            bitmapVector[idx + 0] = static_cast<uint8_t>(
                                (color.blue * src_weight +
                                    bitmapVector[idx + 0] * dst_weight));  // B
                            bitmapVector[idx + 1] = static_cast<uint8_t>(
                                (color.green * src_weight +
                                    bitmapVector[idx + 1] * dst_weight));  // G
                            bitmapVector[idx + 2] = static_cast<uint8_t>(
                                (color.red * src_weight +
                                    bitmapVector[idx + 2] * dst_weight));  // R
                            bitmapVector[idx + 3] = static_cast<uint8_t>(out_a * 255.0f); // A
                        }
                    }
                    //else __debugbreak();

                }
            }
        }

        // Step 5: Create texture from bitmap
        glyph.create(instance, instanceFunctions, functions, device, physicalDevice, {
                glm::ivec2(width, height),
                glm::ivec2(originalLeft, originalTop),
                glm::ivec2(face->glyph->advance.x, face->glyph->advance.y)
            }, character,
            bitmapVector.data(),
            width,
            height,
            width * 4, 
            4,
            Graphics::Format::R8G8B8A8Unorm
        );
        return glyph;
    }

    const Glyph& Font::Size::getGlyph(GUI::Instance& instance, const Graphics::InstanceFunctionTable& instanceFunctions, 
        const Graphics::DeviceFunctionTable& functions, Graphics::DeviceRef device, 
        Graphics::PhysicalDevice physicalDevice, CharId character) const {
            
        auto it = m_glyphMap.find(character);
        if (it != m_glyphMap.end()) return it->second;

        FT_Activate_Size(m_size);

        Glyph glyph;

        switch(m_font->m_type) {
        case FontType::Normal:
            glyph = getGlyphGrayscale(instance, instanceFunctions, functions, device, physicalDevice, character);
            break;
        case FontType::Colored:
            glyph = getGlyphGrayscale(instance, instanceFunctions, functions, device, physicalDevice, character);
            break;
        case FontType::ColoredColrCpal:
            glyph = getGlyphColrCpal(instance, instanceFunctions, functions, device, physicalDevice, character);
            break;
        case FontType::ColoredCbdtCblc:
            glyph = getGlyphCbdtCblc(instance, instanceFunctions, functions, device, physicalDevice, character);
            break;
        case FontType::ColoredSbix:
            glyph = getGlyphSibix(instance, instanceFunctions, functions, device, physicalDevice, character);
            break;
        default: throw std::runtime_error("Unimplemented FontType");
        }

        return m_glyphMap.emplace(character, std::move(glyph)).first->second;
    }
}