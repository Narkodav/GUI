#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"

namespace Graphics
{
	class DeviceRef;

	class SamplerRef : public BaseComponent<VkSampler, SamplerRef>
	{
		using Base = BaseComponent<VkSampler, SamplerRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "Sampler";
	};

	class SamplerCreateInfo : public StructBase<VkSamplerCreateInfo, SamplerCreateInfo>
	{
		using Base = StructBase<VkSamplerCreateInfo, SamplerCreateInfo>;
	public:
		using Base::Base;

		static inline const float s_lodClampNone = VK_LOD_CLAMP_NONE;

		SamplerCreateInfo(Filter magFilter = Filter::Linear, Filter minFilter = Filter::Linear,
			SamplerMipmapMode mipmapMode = SamplerMipmapMode::Linear,
			SamplerAddressMode modeU = SamplerAddressMode::Repeat,
			SamplerAddressMode modeV = SamplerAddressMode::Repeat,
			SamplerAddressMode modeW = SamplerAddressMode::Repeat,
			float mipLodBias = 0.0f,
			bool anisotropyEnable = false,
			float maxAnisotropy = 1.0f,
			bool compareEnable = false,
			CompareOp compareOp = CompareOp::Always,
			float minLod = 0.0f,
			float maxLod = s_lodClampNone,
			BorderColor borderColor = BorderColor::IntOpaqueBlack,
			bool unnormalizedCoordinates = false,
			Flags::SamplerCreate flags = Flags::SamplerCreate::Bits::None)
			: Base()
		{
			this->flags = flags;
			this->magFilter = static_cast<VkFilter>(magFilter);
			this->minFilter = static_cast<VkFilter>(minFilter);
			this->mipmapMode = static_cast<VkSamplerMipmapMode>(mipmapMode);
			this->addressModeU = static_cast<VkSamplerAddressMode>(modeU);
			this->addressModeV = static_cast<VkSamplerAddressMode>(modeV);
			this->addressModeW = static_cast<VkSamplerAddressMode>(modeW);
			this->mipLodBias = mipLodBias;
			this->anisotropyEnable = anisotropyEnable;
			this->maxAnisotropy = maxAnisotropy;
			this->compareEnable = compareEnable;
			this->compareOp = static_cast<VkCompareOp>(compareOp);
			this->minLod = minLod;
			this->maxLod = maxLod;
			this->borderColor = static_cast<VkBorderColor>(borderColor);
			this->unnormalizedCoordinates = unnormalizedCoordinates;
		}

		SamplerCreateInfo& setMagFilter(Filter magFilter) {
			this->magFilter = static_cast<VkFilter>(magFilter);
			return *this;
		}

		SamplerCreateInfo& setMinFilter(Filter minFilter) {
			this->minFilter = static_cast<VkFilter>(minFilter);
			return *this;
		}

		SamplerCreateInfo& setMipmapMode(SamplerMipmapMode mipmapMode) {
			this->mipmapMode = static_cast<VkSamplerMipmapMode>(mipmapMode);
			return *this;
		}

		SamplerCreateInfo& setAddressModeU(SamplerAddressMode modeU) {
			this->addressModeU = static_cast<VkSamplerAddressMode>(modeU);
			return *this;
		}

		SamplerCreateInfo& setAddressModeV(SamplerAddressMode modeV) {
			this->addressModeV = static_cast<VkSamplerAddressMode>(modeV);
			return *this;
		}

		SamplerCreateInfo& setAddressModeW(SamplerAddressMode modeW) {
			this->addressModeW = static_cast<VkSamplerAddressMode>(modeW);
			return *this;
		}

		SamplerCreateInfo& setMipLodBias(float mipLodBias) {
			this->mipLodBias = mipLodBias;
			return *this;
		}

		SamplerCreateInfo& setAnisotropyEnable(bool enable) {
			this->anisotropyEnable = enable;
			return *this;
		}

		SamplerCreateInfo& setMaxAnisotropy(float maxAnisotropy) {
			this->maxAnisotropy = maxAnisotropy;
			return *this;
		}

		SamplerCreateInfo& setCompareEnable(bool enable) {
			this->compareEnable = enable;
			return *this;
		}

		SamplerCreateInfo& setCompareOp(CompareOp compareOp) {
			this->compareOp = static_cast<VkCompareOp>(compareOp);
			return *this;
		}

		SamplerCreateInfo& setMinLod(float minLod) {
			this->minLod = minLod;
			return *this;
		}

		SamplerCreateInfo& setMaxLod(float maxLod) {
			this->maxLod = maxLod;
			return *this;
		}

		SamplerCreateInfo& setBorderColor(BorderColor borderColor) {
			this->borderColor = static_cast<VkBorderColor>(borderColor);
			return *this;
		}

		SamplerCreateInfo& setUnnormalizedCoordinates(bool unnormalizedCoordinates) {
			this->unnormalizedCoordinates = unnormalizedCoordinates;
			return *this;
		}

        // ===================================================================
        // Default sampler for general-purpose 3D textured surfaces
        // Use for: diffuse/albedo textures, specular maps, roughness maps
        // Why: Linear filtering provides smooth visuals, anisotropy reduces
        //      blurring on angled surfaces, repeat addressing allows tiling
        // ===================================================================
        static SamplerCreateInfo defaultSampler()
        {
            return SamplerCreateInfo(
                Filter::Linear,                    // magFilter: Smooth when texture is magnified
                Filter::Linear,                    // minFilter: Smooth when texture is minified
                SamplerMipmapMode::Linear,         // mipmapMode: Trilinear filtering between mip levels
                SamplerAddressMode::Repeat,        // modeU: Tile texture horizontally
                SamplerAddressMode::Repeat,        // modeV: Tile texture vertically
                SamplerAddressMode::Repeat,        // modeW: Tile texture in 3D space
                0.0f,                              // mipLodBias: No bias in mip selection
                true,                              // anisotropyEnable: Reduce blur on angled surfaces
                8.0f,                              // maxAnisotropy: Good quality/performance balance
                false,                             // compareEnable: Not a depth/shadow texture
                CompareOp::Always,                 // compareOp: N/A (compare disabled)
                0.0f,                              // minLod: Allow highest resolution mip
                s_lodClampNone,                    // maxLod: Allow all mip levels
                BorderColor::IntOpaqueBlack,       // borderColor: Default border (not used with repeat)
                false                              // unnormalizedCoordinates: Use [0,1] UV coordinates
            );
        }

        // ===================================================================
        // Sampler for pixel art, retro games, or textures requiring crisp edges
        // Use for: Sprite sheets, tilemaps, UI icons with pixel art aesthetic
        // Why: Nearest filtering preserves sharp pixel boundaries, clamp-to-edge
        //      prevents color bleeding from adjacent tiles, no anisotropy needed
        // ===================================================================
        static SamplerCreateInfo pixelArtSampler() {
            return SamplerCreateInfo(
                Filter::Nearest,                   // magFilter: Preserve pixel boundaries when enlarged
                Filter::Nearest,                   // minFilter: Preserve pixel boundaries when reduced
                SamplerMipmapMode::Nearest,        // mipmapMode: Use exact mip levels (no blending)
                SamplerAddressMode::ClampToEdge,   // modeU: Prevent color bleeding from adjacent sprites
                SamplerAddressMode::ClampToEdge,   // modeV: Same for vertical direction
                SamplerAddressMode::ClampToEdge,   // modeW: Same for 3D textures
                0.0f,                              // mipLodBias: No bias needed for pixel art
                false,                             // anisotropyEnable: Would blur crisp pixel edges
                1.0f,                              // maxAnisotropy: Disabled (1.0 = off)
                false,                             // compareEnable: Not a depth texture
                CompareOp::Always,                 // compareOp: N/A
                0.0f,                              // minLod: Start from highest detail
                s_lodClampNone,                    // maxLod: Use all mips (if texture has them)
                BorderColor::IntTransparentBlack,  // borderColor: Transparent for sprite edges
                false                              // unnormalizedCoordinates: Use normalized UVs
            );
        }

        // ===================================================================
        // Sampler for environment maps, skyboxes, and cube maps
        // Use for: Sky textures, environment reflections, irradiance maps
        // Why: Clamp-to-edge prevents seam artifacts at cube edges,
        //      maxLod = 0 forces base mip to keep distant features sharp
        // ===================================================================
        static SamplerCreateInfo skyboxSampler() {
            return SamplerCreateInfo(
                Filter::Linear,                    // magFilter: Smooth for nearby sky details
                Filter::Linear,                    // minFilter: Smooth for distant sky regions
                SamplerMipmapMode::Linear,         // mipmapMode: Smooth transitions if mips exist
                SamplerAddressMode::ClampToEdge,   // modeU: Critical! Prevents seams at cube edges
                SamplerAddressMode::ClampToEdge,   // modeV: Same for vertical direction
                SamplerAddressMode::ClampToEdge,   // modeW: Same for cube map faces
                0.0f,                              // mipLodBias: No bias for skybox
                true,                              // anisotropyEnable: Improve quality on angled horizon
                4.0f,                              // maxAnisotropy: Moderate for performance
                false,                             // compareEnable: Not a depth texture
                CompareOp::Always,                 // compareOp: N/A
                0.0f,                              // minLod: Highest detail mip
                0.0f,                              // maxLod: CRITICAL: Force base mip only (keeps stars/clouds sharp)
                BorderColor::IntOpaqueBlack,       // borderColor: Black for any sampling outside
                false                              // unnormalizedCoordinates: Normalized coordinates
            );
        }

        // ===================================================================
        // Sampler for shadow mapping with Percentage Closer Filtering (PCF)
        // Use for: Shadow maps, depth comparisons
        // Why: Compare enabled for depth testing, clamp-to-border with white
        //      treats areas outside shadow map as fully lit
        // ===================================================================
        static SamplerCreateInfo shadowMapSampler() {
            return SamplerCreateInfo(
                Filter::Linear,                    // magFilter: Linear for soft shadow edges (PCF)
                Filter::Linear,                    // minFilter: Linear for distant shadows
                SamplerMipmapMode::Nearest,        // mipmapMode: Use exact mip (no blending between depth levels)
                SamplerAddressMode::ClampToBorder, // modeU: Treat sampling outside map as border color
                SamplerAddressMode::ClampToBorder, // modeV: Same for vertical
                SamplerAddressMode::ClampToBorder, // modeW: Same for 3D shadow maps
                0.0f,                              // mipLodBias: No bias for depth comparison
                false,                             // anisotropyEnable: Not useful for depth textures
                1.0f,                              // maxAnisotropy: Disabled
                true,                              // compareEnable: CRITICAL: Enables depth comparison
                CompareOp::LessOrEqual,            // compareOp: Depth test - pass if fragment depth <= shadow map depth
                0.0f,                              // minLod: Start from highest detail depth map
                s_lodClampNone,                    // maxLod: Use all depth mips
                BorderColor::FloatOpaqueWhite,     // borderColor: CRITICAL: Areas outside shadow map are fully lit
                false                              // unnormalizedCoordinates: Normalized depth coordinates
            );
        }

        // ===================================================================
        // Sampler for normal mapping to preserve surface detail
        // Use for: Normal maps, bump maps, displacement maps
        // Why: Limited maxLod (0.5) prevents noisy artifacts at distance,
        //      anisotropy helps maintain detail on angled surfaces
        // ===================================================================
        static SamplerCreateInfo normalMapSampler() {
            return SamplerCreateInfo(
                Filter::Linear,                    // magFilter: Smooth normal interpolation up close
                Filter::Linear,                    // minFilter: Smooth normal interpolation at distance
                SamplerMipmapMode::Linear,         // mipmapMode: Blend normals smoothly between mips
                SamplerAddressMode::Repeat,        // modeU: Tile normals across surface
                SamplerAddressMode::Repeat,        // modeV: Same for vertical
                SamplerAddressMode::Repeat,        // modeW: For 3D normal maps
                0.0f,                              // mipLodBias: No bias needed
                true,                              // anisotropyEnable: Preserve normal detail on slopes
                4.0f,                              // maxAnisotropy: Moderate for normal detail
                false,                             // compareEnable: Not a depth texture
                CompareOp::Always,                 // compareOp: N/A
                0.0f,                              // minLod: Highest detail normals
                0.5f,                              // maxLod: CRITICAL: Limit to half resolution to avoid noisy normals at distance
                BorderColor::IntOpaqueBlack,       // borderColor: Flat (0,0,1) normal at borders
                false                              // unnormalizedCoordinates: Normalized UVs
            );
        }

        // ===================================================================
        // Sampler for UI elements, text, and 2D overlays
        // Use for: Font textures, UI icons, HUD elements
        // Why: No mipmapping (maxLod = 0) keeps text sharp,
        //      clamp-to-edge prevents bleeding, transparent borders
        // ===================================================================
        static SamplerCreateInfo uiSampler() {
            return SamplerCreateInfo(
                Filter::Linear,                    // magFilter: Smooth when UI is scaled up
                Filter::Linear,                    // minFilter: Smooth when UI is scaled down
                SamplerMipmapMode::Linear,         // mipmapMode: If mips exist, blend them
                SamplerAddressMode::ClampToEdge,   // modeU: Prevent glyph bleeding in font atlases
                SamplerAddressMode::ClampToEdge,   // modeV: Same for vertical
                SamplerAddressMode::ClampToEdge,   // modeW: For 3D UI elements (rare)
                0.0f,                              // mipLodBias: No bias for UI
                false,                             // anisotropyEnable: UI is typically viewed head-on
                1.0f,                              // maxAnisotropy: Disabled
                false,                             // compareEnable: Not a depth texture
                CompareOp::Always,                 // compareOp: N/A
                0.0f,                              // minLod: Highest detail
                0.0f,                              // maxLod: CRITICAL: No mipmapping - keeps UI text/elements sharp
                BorderColor::IntTransparentBlack,  // borderColor: Transparent for clean UI edges
                false                              // unnormalizedCoordinates: Normalized UVs
            );
        }

        // ===================================================================
        // Special sampler for compute shader texel fetches
        // Use for: Compute shaders doing image processing, compute-based rendering
        // Why: Unnormalized coordinates allow direct texel addressing,
        //      nearest filtering for exact texel values
        // ===================================================================
        static SamplerCreateInfo computeShaderSampler() {
            return SamplerCreateInfo(
                Filter::Nearest,                   // magFilter: Exact texel values, no interpolation
                Filter::Nearest,                   // minFilter: Exact texel values
                SamplerMipmapMode::Nearest,        // mipmapMode: Specific mip level selection
                SamplerAddressMode::ClampToEdge,   // modeU: Prevent out-of-bounds access
                SamplerAddressMode::ClampToEdge,   // modeV: Same for vertical
                SamplerAddressMode::ClampToEdge,   // modeW: For 3D textures in compute
                0.0f,                              // mipLodBias: No bias for compute operations
                false,                             // anisotropyEnable: Not relevant for texel fetches
                1.0f,                              // maxAnisotropy: Disabled
                false,                             // compareEnable: Not depth comparison
                CompareOp::Always,                 // compareOp: N/A
                0.0f,                              // minLod: Start from base mip
                s_lodClampNone,                    // maxLod: Allow all mips if needed
                BorderColor::IntOpaqueBlack,       // borderColor: Safe default for out-of-bounds
                true                               // unnormalizedCoordinates: CRITICAL: Use texel coordinates [0,width-1]
            );
        }

        // ===================================================================
        // Sampler for terrain height maps and detailed surfaces
        // Use for: Height maps, terrain textures, large tiled surfaces
        // Why: High anisotropy (16x) for distant terrain, negative LOD bias
        //      for sharper distant features, limited maxLod prevents over-blurring
        // ===================================================================
        static SamplerCreateInfo heightMapSampler() {
            return SamplerCreateInfo(
                Filter::Linear,                    // magFilter: Smooth height interpolation up close
                Filter::Linear,                    // minFilter: Smooth height interpolation at distance
                SamplerMipmapMode::Linear,         // mipmapMode: Blend height values between mips
                SamplerAddressMode::Repeat,        // modeU: Tile height map across terrain
                SamplerAddressMode::Repeat,        // modeV: Same for vertical
                SamplerAddressMode::Repeat,        // modeW: For 3D noise textures
                -0.5f,                             // mipLodBias: Bias toward sharper mips (less blur at distance)
                true,                              // anisotropyEnable: CRITICAL: Maintain detail on sloped terrain
                16.0f,                             // maxAnisotropy: High for distant terrain quality
                false,                             // compareEnable: Not a depth texture
                CompareOp::Always,                 // compareOp: N/A
                0.0f,                              // minLod: Highest detail
                4.0f,                              // maxLod: Limit blur at extreme distances
                BorderColor::IntOpaqueBlack,       // borderColor: Default height (0) at borders
                false                              // unnormalizedCoordinates: Normalized coordinates
            );
        }

        // ===================================================================
        // Sampler for 1D lookup tables (color grading, tone mapping, etc.)
        // Use for: Color LUTs, tone mapping curves, remapping tables
        // Why: Linear filtering for smooth interpolation between table entries,
        //      clamp-to-edge to stay within valid LUT range, no mipmapping needed
        // ===================================================================
        static SamplerCreateInfo lookupTable1DSampler() {
            return SamplerCreateInfo(
                Filter::Linear,                    // magFilter: Smooth interpolation between LUT entries
                Filter::Linear,                    // minFilter: Same (though 1D textures don't truly minify)
                SamplerMipmapMode::Nearest,        // mipmapMode: LUTs don't have mipmaps
                SamplerAddressMode::ClampToEdge,   // modeU: Stay within valid LUT range [0,1]
                SamplerAddressMode::ClampToEdge,   // modeV: N/A for 1D but required
                SamplerAddressMode::ClampToEdge,   // modeW: N/A for 1D but required
                0.0f,                              // mipLodBias: No bias for LUTs
                false,                             // anisotropyEnable: Not relevant for 1D textures
                1.0f,                              // maxAnisotropy: Disabled
                false,                             // compareEnable: Not a depth texture
                CompareOp::Always,                 // compareOp: N/A
                0.0f,                              // minLod: Base mip only
                0.0f,                              // maxLod: CRITICAL: No mipmapping for LUTs
                BorderColor::IntOpaqueBlack,       // borderColor: Default value at LUT boundaries
                false                              // unnormalizedCoordinates: Use normalized [0,1] range
            );
        }


	};

	class Sampler : public VerificatorComponent<VkSampler, SamplerRef>
	{
		using Base = VerificatorComponent<VkSampler, SamplerRef>;
	public:
		using Base::Base;

		void create(const DeviceFunctionTable& functions, const DeviceRef& device, const SamplerCreateInfo& createInfo);
		void destroy(const DeviceFunctionTable& functions, const DeviceRef& device);
	};
}

