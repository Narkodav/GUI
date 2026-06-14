#pragma once
#include "Enums.h"
#include "Flags.h"
#include "TaskTables/FeatureEnum.h"
#include "TaskTables/PropertyEnum.h"
#include "TaskTables/QueuePropertyEnum.h"
#include "Common.h"
#include "PlatformKit/Window.h"
#include "FunctionPointers.h"

namespace Graphics {

    // template<typename T, size_t N>
    // static constexpr void constCopySpanToArray(std::array<T, N> dst, std::span<const T, N> src) {
    //     if constexpr (std::is_constant_evaluated()) {
    //         for (size_t i = 0; i < N; ++i) dst[i] = src[i];
    //     }
    //     else {
    //         std::copy(src.begin(), src.end(), dst.begin());
    //     }
    // }

    struct Version
    {
        uint32_t version;

        operator uint32_t() const
        {
            return version;
        }

        operator const uint32_t& () const
        {
            return version;
        }

        operator uint32_t& ()
        {
            return version;
        }

        Version(uint32_t maj, uint32_t min, uint32_t pat)
            : version(VK_MAKE_VERSION(maj, min, pat)) {
        }

        Version(uint32_t version)
            : version(version) {
        };

        uint32_t getVersion() const
        {
            return version;
        }

        uint32_t major() const
        {
            return VK_VERSION_MAJOR(version);
        }

        uint32_t minor() const
        {
            return VK_VERSION_MINOR(version);
        }

        uint32_t patch() const
        {
            return VK_VERSION_PATCH(version);
        }
    };

    struct BufferCopy : public StructBase<VkBufferCopy, BufferCopy> {
        using Base = StructBase<VkBufferCopy, BufferCopy>;
    public:
        using Base::Base;

        BufferCopy(size_t srcOffset, size_t dstOffset, size_t size) : Base() {
            this->dstOffset = dstOffset;
            this->srcOffset = srcOffset;
            this->size = size;
        };

        BufferCopy& setSrcOffset(size_t srcOffset) { this->srcOffset = srcOffset; return *this; }
        BufferCopy& setDstOffset(size_t dstOffset) { this->dstOffset = dstOffset; return *this; }
        BufferCopy& setSize(size_t size) { this->size = size; return *this; }
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::BufferCopy, VkBufferCopy);

    struct alignas(4) DrawCommand
    {
        uint32_t vertexCount;
        uint32_t instanceCount;
        uint32_t firstVertex;
        uint32_t firstInstance;
    };

    struct Extent2D : public StructBase<VkExtent2D, Extent2D> {
        using Base = StructBase<VkExtent2D, Extent2D>;
    public:
        using Base::Base;

        Extent2D(uint32_t w, uint32_t h) : Base() {
            this->width = w;
            this->height = h;
        }

        Extent2D(PlatformKit::Extent extent) : Base() {
            this->width = extent.width;
            this->height = extent.height;
        }

        Extent2D& setWidth(uint32_t w) { this->width = w; return *this; }
        Extent2D& setHeight(uint32_t h) { this->height = h; return *this; }

        uint32_t getWidth() const { return this->width; };
        uint32_t getHeight() const { return this->height; };
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::Extent2D, VkExtent2D);

    struct Offset2D : public StructBase<VkOffset2D, Offset2D>
    {
        using Base = StructBase<VkOffset2D, Offset2D>;
    public:
        using Base::Base;

        Offset2D(int32_t x = 0, int32_t y = 0) : Base() {
            this->x = x;
            this->y = y;
        }

        //operator const glm::ivec2& () const { return *reinterpret_cast<const glm::ivec2*>(this); };
        //operator glm::ivec2& () { return *reinterpret_cast<glm::ivec2*>(this); };

        Offset2D& setX(int32_t x) {
            this->x = x;
            return *this;
        }

        Offset2D& setY(int32_t y) {
            this->y = y;
            return *this;
        }

        int32_t getX() const { return this->x; };
        int32_t getY() const { return this->y; };
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::Offset2D, VkOffset2D);

    class Extent3D : public StructBase<VkExtent3D, Extent3D>
    {
        using Base = StructBase<VkExtent3D, Extent3D>;
    public:
        using Base::Base;

        Extent3D(uint32_t w, uint32_t h, uint32_t d) : Base() {
            this->width = w;
            this->height = h;
            this->depth = d;
        }

        Extent3D(const Extent2D& extent, uint32_t d) : Base() {
            this->width = extent.getWidth();
            this->height = extent.getHeight();
            this->depth = d;
        }

        Extent3D& setWidth(uint32_t w) {
            this->width = w;
            return *this;
        }

        Extent3D& setHeight(uint32_t h) {
            this->height = h;
            return *this;
        }

        Extent3D& setDepth(uint32_t d) {
            this->depth = d;
            return *this;
        }

        uint32_t getWidth() const { return this->width; };
        uint32_t getHeight() const { return this->height; };
        uint32_t getDepth() const { return this->depth; };
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::Extent3D, VkExtent3D);

    struct Offset3D : public StructBase<VkOffset3D, Offset3D>
    {
        using Base = StructBase<VkOffset3D, Offset3D>;
    public:
        using Base::Base;

        Offset3D(int32_t x = 0, int32_t y = 0, int32_t z = 0) : Base() {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        //operator const glm::ivec3& () const { return *reinterpret_cast<const glm::ivec3*>(this); };
        //operator glm::ivec3& () { return *reinterpret_cast<glm::ivec3*>(this); };

        Offset3D& setX(int32_t x) {
            this->x = x;
            return *this;
        }

        Offset3D& setY(int32_t y) {
            this->y = y;
            return *this;
        }

        Offset3D& setZ(int32_t z) {
            this->z = z;
            return *this;
        }

        int32_t getX() const { return this->x; };
        int32_t getY() const { return this->y; };
        int32_t getZ() const { return this->z; };
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::Offset3D, VkOffset3D);

    struct Viewport : public StructBase<VkViewport, Viewport>
    {
        using Base = StructBase<VkViewport, Viewport>;
    public:
        using Base::Base;

        Viewport(float x, float y, float width, float height,
            float minDepth, float maxDepth) : Base() {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            this->minDepth = minDepth;
            this->maxDepth = maxDepth;
        }

        int32_t getX() const { return this->x; };
        int32_t getY() const { return this->y; };
        int32_t getWidth() const { return this->width; };
        int32_t getHeight() const { return this->height; };
        int32_t getMinDepth() const { return this->minDepth; };
        int32_t getMaxDepth() const { return this->maxDepth; };

        Viewport& setX(float x) { this->x = x; return *this; };
        Viewport& setY(float y) { this->y = y; return *this; };
        Viewport& setWidth(float width) { this->width = width; return *this; };
        Viewport& setHeight(float height) { this->height = height; return *this; };
        Viewport& setMinDepth(float minDepth) { this->minDepth = minDepth; return *this; };
        Viewport& setMaxDepth(float maxDepth) { this->maxDepth = maxDepth; return *this; };
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::Viewport, VkViewport);

    class Rect2D : public StructBase<VkRect2D, Rect2D>
    {
        using Base = StructBase<VkRect2D, Rect2D>;
    public:
        using Base::Base;

        Rect2D(const Offset2D& offset, const Extent2D& extent) : Base() {
            this->offset.x = offset.getX();
            this->offset.y = offset.getY();
            this->extent.width = extent.getWidth();
            this->extent.height = extent.getHeight();
        };

        Rect2D(const Extent2D& extent) : Base() {
            this->offset.x = 0;
            this->offset.y = 0;
            this->extent.width = extent.getWidth();
            this->extent.height = extent.getHeight();
        };

        const Offset2D& getOffset() const {
			return Offset2D::underlyingCast(this->offset);
        };

        const Extent2D& getExtent() const {
			return Extent2D::underlyingCast(this->extent);
        };

        Rect2D& setOffset(const Offset2D& offset) {
            this->offset = offset;
            return *this;
        };

        Rect2D& setExtent(const Extent2D& extent) {
            this->extent = extent;
            return *this;
		};
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::Rect2D, VkRect2D);

    using Scissor = Rect2D; //for better readability

    struct RenderRegion {
    private:
        Viewport viewport;
        Scissor scissor;

    public:
        RenderRegion() = default;
        ~RenderRegion() = default;

        RenderRegion(const RenderRegion&) noexcept = default;
        RenderRegion(RenderRegion&&) noexcept = default;

        RenderRegion& operator=(const RenderRegion&) = default;
        RenderRegion& operator=(RenderRegion&&) noexcept = default;

        Viewport getViewport() const { return viewport; }
        Rect2D getScissor() const { return scissor; }

        void setViewport(const Viewport& viewport) { this->viewport = viewport; }
        void setScissor(const Scissor& scissor) { this->scissor = scissor; }

        // Create a full-window render region
        static RenderRegion createFullWindow(const Extent2D& extent) {
            RenderRegion region{};

            region.viewport.setX(0.0f)
                .setY(0.0f)
                .setHeight(static_cast<float>(extent.getHeight()))
                .setWidth(static_cast<float>(extent.getWidth()))
                .setMinDepth(0.0f)
                .setMaxDepth(1.0f);

			region.scissor.offset.x = 0;
			region.scissor.offset.y = 0;
			region.scissor.extent = extent;

            return region;
        }

        // Create a custom region within the window
        static RenderRegion createCustom(
            float x, float y,           //region left top corner
            float width, float height   //region dimensions
        )
        {
            RenderRegion region{};
			region.viewport.height = height;
			region.viewport.width = width;
            region.viewport.x = x;
            region.viewport.y = y;
            region.viewport.minDepth = 0.0f;
            region.viewport.maxDepth = 1.0f;

			region.scissor.extent.height = static_cast<uint32_t>(height);
			region.scissor.extent.width = static_cast<uint32_t>(width);
            region.scissor.offset.x = static_cast<int32_t>(x);
			region.scissor.offset.y = static_cast<int32_t>(y);
            return region;
        }

        bool isWithinBounds(const Extent2D& extent) const {
            return (viewport.getX() >= 0.0f &&
                viewport.getY() >= 0.0f &&
                viewport.getX() + viewport.getWidth() <= static_cast<float>(extent.getWidth()) &&
                viewport.getY() + viewport.getHeight() <= static_cast<float>(extent.getHeight()));
        }
    };

    class MemoryRequirements : public StructBase<VkMemoryRequirements, MemoryRequirements>
    {
        using Base = StructBase<VkMemoryRequirements, MemoryRequirements>;
    public:
        using Base::Base;
        size_t getSize() const { return static_cast<size_t>(this->size); }
        size_t getAlignment() const { return static_cast<size_t>(this->alignment); }
        uint32_t getMemoryTypeBits() const { return this->memoryTypeBits; }
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::MemoryRequirements, VkMemoryRequirements);

    class PushConstantRange : public StructBase<VkPushConstantRange, PushConstantRange>
    {
        using Base = StructBase<VkPushConstantRange, PushConstantRange>;
    public:
        using Base::Base;

        PushConstantRange(Flags::ShaderStage stage, uint32_t offset, uint32_t size) : Base() {
            this->stageFlags = stage;
            this->offset = offset;
            this->size = size;
        }

        PushConstantRange& setStageFlags(Flags::ShaderStage stage) {
            this->stageFlags = stage;
            return *this;
        }

        PushConstantRange& setOffset(uint32_t offset) {
            this->offset = offset;
            return *this;
        }

        PushConstantRange& setSize(uint32_t size) {
            this->size = size;
            return *this;
        }

        Flags::ShaderStage getStageFlags() const { return this->stageFlags; };
        uint32_t getOffset() const { return this->offset; };
        uint32_t getSize() const { return this->size; };
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::PushConstantRange, VkPushConstantRange);
    
    class ClearColorValue : public UnionBase<VkClearColorValue, ClearColorValue>
    {
    public:
        union Storage {
            std::array<float, 4> float32;
            std::array<int32_t, 4> int32;
            std::array<uint32_t, 4> uint32;

            constexpr Storage() noexcept {};
            constexpr ~Storage() noexcept = default;

            constexpr Storage(const Storage&) noexcept = default;
            constexpr Storage(Storage&&) noexcept = default;

            constexpr Storage& operator=(const Storage&) noexcept = default;
            constexpr Storage& operator=(Storage&&) noexcept = default;
        } m_storage;

        using Base = UnionBase<VkClearColorValue, ClearColorValue>;
    
        using Base::Base;

        constexpr ClearColorValue() noexcept {};

        constexpr ClearColorValue(std::span<const float, 4> value) noexcept {
            for (size_t i = 0; i < 4; ++i) m_storage.float32[i] = value[i];
        }

        constexpr ClearColorValue(std::span<const int32_t, 4> value) noexcept {
            for (size_t i = 0; i < 4; ++i) m_storage.int32[i] = value[i];
        }

        constexpr ClearColorValue(std::span<const uint32_t, 4> value) noexcept {
            for (size_t i = 0; i < 4; ++i) m_storage.uint32[i] = value[i];
        }

        constexpr ClearColorValue& setClearValue(const std::array<float, 4>& value) {
            m_storage.float32 = value;
            return *this;
        }

        constexpr ClearColorValue& setClearValue(const std::array<int32_t, 4>& value) {
            m_storage.int32 = value;
            return *this;
        }

        constexpr ClearColorValue& setClearValue(const std::array<uint32_t, 4>& value) {
            m_storage.uint32 = value;
            return *this;
        }

        constexpr std::span<const float, 4> getFloat32() const { return m_storage.float32; }
        constexpr std::span<const int32_t, 4> getInt32() const { return m_storage.int32; }
        constexpr std::span<const uint32_t, 4> getUint32() const { return m_storage.uint32; }
    };
    GRAPHICS_ASSERT_UNION_PROPERTIES(Graphics::ClearColorValue, VkClearColorValue);

    class ClearDepthStencilValue : public StructBase<VkClearDepthStencilValue, ClearDepthStencilValue>
    {
        using Base = StructBase<VkClearDepthStencilValue, ClearDepthStencilValue>;
    public:
        using Base::Base;

        ClearDepthStencilValue(float depth, uint32_t stencil) {
            this->depth = depth;
            this->stencil = stencil;
        };

        ClearDepthStencilValue& setDepth(float depth) {
            this->depth = depth;
            return *this;
        }

        ClearDepthStencilValue& setStencil(uint32_t stencil) {
            this->stencil = stencil;
            return *this;
        }

        float getDepth() const { return this->depth; }
        uint32_t getStencil() const { return this->stencil; }
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::ClearDepthStencilValue, VkClearDepthStencilValue);

    class ClearValue : public UnionBase<VkClearValue, ClearValue>
    {
    public:
        union Storage {
            ClearColorValue color;
            ClearDepthStencilValue depthStencil;

            constexpr Storage() noexcept {};
            constexpr ~Storage() noexcept = default;

            constexpr Storage(const Storage&) noexcept = default;
            constexpr Storage(Storage&&) noexcept = default;

            constexpr Storage& operator=(const Storage&) noexcept = default;
            constexpr Storage& operator=(Storage&&) noexcept = default;
        } m_storage;

        using Base = UnionBase<VkClearValue, ClearValue>;
        using Base::Base;

        constexpr ClearValue(const ClearColorValue& value) noexcept {
            m_storage.color = value;
        }

        constexpr ClearValue(const ClearDepthStencilValue& value) noexcept {
            m_storage.depthStencil = value;
        }

        constexpr ClearValue& setClearValue(const ClearColorValue& color) {
            m_storage.color = color;
            return *this;
        }

        constexpr ClearValue& setClearValue(const ClearDepthStencilValue& depthStencil) {
            m_storage.depthStencil = depthStencil;
            return *this;
        }

        constexpr const ClearColorValue& getClearColor() const { return m_storage.color; }
        constexpr const ClearDepthStencilValue& getClearDepthStencil() const { return m_storage.depthStencil; }
    };
    GRAPHICS_ASSERT_UNION_PROPERTIES(Graphics::ClearValue, VkClearValue);

    using ArrayFloat4 = std::array<float, 4>;
    struct Color : public StructBase<ArrayFloat4, Color>
    {
        using Base = StructBase<ArrayFloat4, Color>;
    public:
        using Base::Base;

        Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) : Base({ r, g, b, a }) {};

        Color(uint8_t r, uint8_t g, uint8_t b,
            uint8_t a = std::numeric_limits<uint8_t>::max()) :
            Base({ (static_cast<float>(r) / 255.f),
            (static_cast<float>(g) / 255.f),
            (static_cast<float>(b) / 255.f),
            (static_cast<float>(a) / 255.f) }) {
        };

        static Color empty() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }
        static Color black() { return Color(0.0f, 0.0f, 0.0f); }
        static Color white() { return Color(1.0f, 1.0f, 1.0f); }
        static Color red() { return Color(1.0f, 0.0f, 0.0f); }
        static Color green() { return Color(0.0f, 1.0f, 0.0f); }
        static Color blue() { return Color(0.0f, 0.0f, 1.0f); }

        Color& setR(float r) {
            Base::operator[](0) = r;
            return *this;
        }

        Color& setG(float g) {
            Base::operator[](1) = g;
            return *this;
        }

        Color& setB(float b) {
            Base::operator[](2) = b;
            return *this;
        }

        Color& setA(float a) {
            Base::operator[](3) = a;
            return *this;
        }

        float getR() const { return Base::operator[](0); };
        float getG() const { return Base::operator[](1); };
        float getB() const { return Base::operator[](2); };
        float getA() const { return Base::operator[](3); };
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::Color, ArrayFloat4);

    class AllocationCallbacks : public StructBase<VkAllocationCallbacks, AllocationCallbacks>
    {
        using Base = StructBase<VkAllocationCallbacks, AllocationCallbacks>;
    public:
        using Base::Base;

        AllocationCallbacks(
            void* pUserData,
            PFN_allocationFunction pfnAllocation,
            PFN_reallocationFunction pfnReallocation,
            PFN_freeFunction pfnFree,
            PFN_internalAllocationNotification pfnInternalAllocation,
            PFN_internalFreeNotification pfnInternalFree
        ){
            this->pUserData = pUserData;
            this->pfnAllocation = reinterpret_cast<PFN_vkAllocationFunction>(pfnAllocation);
            this->pfnReallocation = reinterpret_cast<PFN_vkReallocationFunction>(pfnReallocation);
            this->pfnFree = pfnFree;
            this->pfnInternalAllocation = reinterpret_cast<PFN_vkInternalAllocationNotification>(pfnInternalAllocation);
            this->pfnInternalFree = reinterpret_cast<PFN_vkInternalFreeNotification>(pfnInternalFree);
        }

        AllocationCallbacks& setUserData(void* pUserData) {
            this->pUserData = pUserData;
            return *this;
        }
        AllocationCallbacks& setAllocationFunction(PFN_allocationFunction pfnAllocation) {
            this->pfnAllocation = reinterpret_cast<PFN_vkAllocationFunction>(pfnAllocation);
            return *this;
        }
        AllocationCallbacks& setReallocationFunction(PFN_reallocationFunction pfnReallocation) {
            this->pfnReallocation = reinterpret_cast<PFN_vkReallocationFunction>(pfnReallocation);
            return *this;
        }
        AllocationCallbacks& setFreeFunction(PFN_freeFunction pfnFree) {
            this->pfnFree = pfnFree;
            return *this;
        }
        AllocationCallbacks& setInternalAllocationNotification(PFN_internalAllocationNotification pfnInternalAllocation) {
            this->pfnInternalAllocation = reinterpret_cast<PFN_vkInternalAllocationNotification>(pfnInternalAllocation);
            return *this;
        }
        AllocationCallbacks& setInternalFreeNotification(PFN_internalFreeNotification pfnInternalFree) {
            this->pfnInternalFree = reinterpret_cast<PFN_vkInternalFreeNotification>(pfnInternalFree);
            return *this;
        }
        void* getUserData() const { return this->pUserData; }
        PFN_allocationFunction getAllocationFunction() const { return reinterpret_cast<PFN_allocationFunction>(this->pfnAllocation); }
        PFN_reallocationFunction getReallocationFunction() const { return reinterpret_cast<PFN_reallocationFunction>(this->pfnReallocation); }
        PFN_freeFunction getFreeFunction() const { return this->pfnFree; }
        PFN_internalAllocationNotification getInternalAllocationNotification() const { return reinterpret_cast<PFN_internalAllocationNotification>(this->pfnInternalAllocation); }
        PFN_internalFreeNotification getInternalFreeNotification() const { return reinterpret_cast<PFN_internalFreeNotification>(this->pfnInternalFree); }
    };
    GRAPHICS_ASSERT_STRUCT_PROPERTIES(Graphics::AllocationCallbacks, VkAllocationCallbacks);
}