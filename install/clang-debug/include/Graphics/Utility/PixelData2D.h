#pragma once
#include "Graphics/Common.h"
#include "Graphics/Structs.h"

// a convenience wrapper for storing pixel data
// since it is a utility wrapper we can implement RAII
namespace Graphics::Utility
{
	class PixelData2D
	{
	private:
		size_t m_width = 0;
		size_t m_height = 0;
		size_t m_bpp = 0;
		size_t m_capacity = 0; //image size in bytes

		uint8_t* m_pixels = nullptr;

	public:
        PixelData2D() : m_width(0), m_height(0), m_bpp(0), m_capacity(0), m_pixels(nullptr) {};

        PixelData2D(std::string_view filepath);

        PixelData2D(PixelData2D&& other) noexcept {
            m_width = std::exchange(other.m_width, 0);
            m_height = std::exchange(other.m_height, 0);
            m_bpp = std::exchange(other.m_bpp, 0);
            m_capacity = std::exchange(other.m_capacity, 0);
            m_pixels = std::exchange(other.m_pixels, nullptr);
        };

        PixelData2D& operator=(PixelData2D&& other) noexcept
        {
            if (this == &other)
                return *this;

            destroy();

            m_width = std::exchange(other.m_width, 0);
            m_height = std::exchange(other.m_height, 0);
            m_bpp = std::exchange(other.m_bpp, 0);
            m_capacity = std::exchange(other.m_capacity, 0);
            m_pixels = std::exchange(other.m_pixels, nullptr);

            return *this;
        };

        PixelData2D(const PixelData2D& other) noexcept = delete;
        PixelData2D& operator=(const PixelData2D& other) noexcept = delete;

        ~PixelData2D() { destroy(); };

        void load(std::string_view filepath);

        std::span<uint8_t> getPixelData() const { return  std::span<uint8_t>(m_pixels, m_capacity); };
        size_t getCapacity() const { return m_capacity; };
        size_t getWidth() const { return m_width; };
        size_t getHeight() const { return m_height; };
        Extent3D getExtent3D() const { return Extent3D(m_width, m_height, 1); };
        Extent2D getExtent2D() const { return Extent2D(m_width, m_height); };

        void destroy();

	};
}

