#pragma once
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#include <type_traits>

namespace Graphics::Flags
{
    template <typename Definition>
    class FlagsBase
    {
    protected:
        uint32_t m_flags;

    public:
        using Bits = typename Definition::Bits;
        using VulkanFlags = typename Definition::VulkanFlags;
        using VulkanBits = typename Definition::VulkanBits;
        using VulkanCFlags = typename Definition::VulkanCFlags;
        using VulkanCBits = typename Definition::VulkanCBits;

        constexpr FlagsBase() : m_flags(0) {};
        constexpr FlagsBase(Bits bit) : m_flags(static_cast<uint32_t>(bit)) {}
        constexpr FlagsBase(uint32_t flags) : m_flags(flags) {}

        template<typename T>
        constexpr FlagsBase(T flags) requires (!std::is_void_v<T>&& std::convertible_to<T, VulkanFlags>) : m_flags(static_cast<uint32_t>(flags)) {};

        template<typename T>
        constexpr FlagsBase(T bit) requires (!std::is_void_v<T>&& std::convertible_to<T, VulkanBits>) : m_flags(static_cast<uint32_t>(bit)) {};

        template<typename T>
        constexpr FlagsBase(T value) requires (!std::is_void_v<T>&& std::convertible_to<T, VulkanCBits>) { m_flags = static_cast<uint32_t>(value); }

        constexpr operator VulkanFlags() const requires (!std::is_void_v<VulkanFlags>) { return VulkanFlags(this->m_flags); }
        constexpr operator VulkanBits() const requires (!std::is_void_v<VulkanBits>) { return static_cast<VulkanBits>(this->m_flags); }

        constexpr operator uint32_t() const { return m_flags; }

        constexpr inline FlagsBase operator|(const Bits& other) const {
            return static_cast<FlagsBase>(m_flags | static_cast<uint32_t>(other));
        }

        constexpr inline FlagsBase operator|(const FlagsBase& other) const {
            return static_cast<FlagsBase>(m_flags | other.m_flags);
        }

        constexpr inline FlagsBase& operator|=(const Bits& other) {
            m_flags |= static_cast<uint32_t>(other);
            return *this;
        }

        constexpr inline FlagsBase& operator|=(const FlagsBase& other) {
            m_flags |= static_cast<uint32_t>(other);
            return *this;
        }

        constexpr inline FlagsBase operator&(const Bits& other) const {
            return static_cast<FlagsBase>(m_flags & static_cast<uint32_t>(other));
        }

        constexpr inline FlagsBase operator&(const FlagsBase& other) const {
            return static_cast<FlagsBase>(m_flags & other.m_flags);
        }

        constexpr inline FlagsBase& operator&=(const Bits& other) {
            m_flags &= static_cast<uint32_t>(other);
            return *this;
        }

        constexpr inline FlagsBase& operator&=(const FlagsBase& other) {
            m_flags &= static_cast<uint32_t>(other);
            return *this;
        }

        constexpr inline bool operator==(const FlagsBase& other) const {
            return m_flags == static_cast<uint32_t>(other);
        }

        constexpr inline bool operator!=(const FlagsBase& other) const {
            return m_flags != static_cast<uint32_t>(other);
        }

        constexpr inline bool operator==(const Bits& other) const {
            return m_flags == static_cast<uint32_t>(other);
        }

        constexpr inline bool operator!=(const Bits& other) const {
            return m_flags != static_cast<uint32_t>(other);
        }

        constexpr inline bool operator==(const uint32_t& other) const {
            return m_flags == other;
        }

        constexpr inline bool operator!=(const uint32_t& other) const {
            return m_flags != other;
        }

        constexpr bool hasFlag(const uint32_t& flag) const {
            return (m_flags & flag) == flag;
        }

        constexpr bool hasFlag(const Bits& flag) const {
            return (m_flags & static_cast<uint32_t>(flag)) == static_cast<uint32_t>(flag);
        }

        constexpr bool hasFlags(const FlagsBase& flags) const {
            return (m_flags & flags.m_flags) == flags.m_flags;
        }

        constexpr bool doesntHaveFlag(const Bits& flag) const {
            return (m_flags & static_cast<uint32_t>(flag)) == 0;
        }

        constexpr bool doesntHaveFlags(const FlagsBase& flags) const {
            return (m_flags & flags.m_flags) == 0;
        }

        constexpr void clear() { m_flags = 0; };

        static size_t flagToIndex(Bits flag) requires requires {
            Definition::s_bitCount;
        } {
			size_t index = 0;
			uint32_t flagValue = static_cast<uint32_t>(flag);
            while (flagValue >>= 1) ++index;
			return index;
        }

        static consteval size_t bitCount() requires requires {
            Definition::s_bitCount;
        } {
            return Definition::s_bitCount;
        }
    };

    template<typename T>
    struct BitTraits;

    template<typename Bits>
    constexpr typename BitTraits<Bits>::ParentType::VulkanBits convertBit(Bits bits) requires
        (!std::is_void_v<typename BitTraits<Bits>::ParentType::VulkanBits>) {
        return static_cast<typename BitTraits<Bits>::ParentType::VulkanBits>(bits);
    }

    template<typename Bits>
    constexpr typename BitTraits<Bits>::ParentType::VulkanCBits convertCBit(Bits bits) requires
        (!std::is_void_v<typename BitTraits<Bits>::ParentType::VulkanCBits>) {
        return static_cast<typename BitTraits<Bits>::ParentType::VulkanCBits>(bits);
    }

    namespace Traits {

        template <typename Bits>
        constexpr inline typename BitTraits<Bits>::ParentType operator|(const Bits& a, const Bits& b) {
            return static_cast<uint32_t>(a) | static_cast<uint32_t>(b);
        }

        template <typename Bits>
        constexpr inline typename BitTraits<Bits>::ParentType operator&(const Bits& a, const Bits& b) {
            return static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
        }

        template <typename Bits>
        constexpr inline bool operator==(const Bits& a, const FlagsBase<typename BitTraits<Bits>::ParentType>& b) {
            return static_cast<uint32_t>(a) == static_cast<uint32_t>(b);
        }

        template <typename Bits>
        constexpr inline bool operator!=(const Bits& a, const FlagsBase<typename BitTraits<Bits>::ParentType>& b) {
            return static_cast<uint32_t>(a) != static_cast<uint32_t>(b);
        }

        template <typename Bits>
        constexpr inline bool operator==(const uint32_t& a, const FlagsBase<typename BitTraits<Bits>::ParentType>& b) {
            return a == static_cast<uint32_t>(b);
        }

        template <typename Bits>
        constexpr inline bool operator!=(const uint32_t& a, const FlagsBase<typename BitTraits<Bits>::ParentType>& b) {
            return a != static_cast<uint32_t>(b);
        }

        template <typename Bits>
        constexpr inline bool operator==(const uint32_t& a, const Bits& b) requires std::is_enum_v<Bits> {
            return a == static_cast<uint32_t>(b);
        }

        template <typename Bits>
        constexpr inline bool operator!=(const uint32_t& a, const Bits& b) requires std::is_enum_v<Bits> {
            return a != static_cast<uint32_t>(b);
        }

        template <typename Bits>
        constexpr inline bool operator==(const Bits& a, const uint32_t& b) requires std::is_enum_v<Bits> {
            return b == static_cast<uint32_t>(a);
        }

        template <typename Bits>
        constexpr inline bool operator!=(const Bits& a, const uint32_t& b) requires std::is_enum_v<Bits> {
            return b != static_cast<uint32_t>(a);
        }
    }

    namespace Traits {
        struct MemoryProperty
        {
            enum class Bits : uint32_t {
                None = 0,
                DeviceLocal       = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                HostVisible       = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                HostCoherent      = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                HostCached        = VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
                LazilyAllocated   = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT,
                Protected         = VK_MEMORY_PROPERTY_PROTECTED_BIT,
                DeviceCoherentAMD = VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD,
                DeviceUncachedAMD = VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD,
                RdmaCapableNV     = VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV,

                HostVisibleCoherent = static_cast<uint32_t>(HostVisible) | static_cast<uint32_t>(HostCoherent),
                DeviceLocalVisible = static_cast<uint32_t>(DeviceLocal) | static_cast<uint32_t>(HostVisible),
            };

			static inline const size_t s_bitCount = 6;

            using VulkanFlags = vk::MemoryPropertyFlags;
            using VulkanBits = vk::MemoryPropertyFlagBits;
            using VulkanCFlags = VkMemoryPropertyFlags;
            using VulkanCBits = VkMemoryPropertyFlagBits;
        };
        struct MemoryHeap
        {
            enum class Bits : uint32_t {
                None = 0,
                DeviceLocal      = VK_MEMORY_HEAP_DEVICE_LOCAL_BIT,
                MultiInstance    = VK_MEMORY_HEAP_MULTI_INSTANCE_BIT,
                MultiInstanceKHR = VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR
            };

            using VulkanFlags = vk::MemoryHeapFlags;
            using VulkanBits = vk::MemoryHeapFlagBits;
            using VulkanCFlags = VkMemoryHeapFlags;
            using VulkanCBits = VkMemoryHeapFlagBits;
        };

        struct BufferUsage
        {
            enum class Bits : uint32_t
            {
                None = 0,
                TransferSrc                                = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                TransferDst                                = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                UniformTexelBuffer                         = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
                StorageTexelBuffer                         = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT,
                UniformBuffer                              = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                StorageBuffer                              = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                IndexBuffer                                = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VertexBuffer                               = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                IndirectBuffer                             = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
                ShaderDeviceAddress                        = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
                ShaderDeviceAddressEXT                     = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_EXT,
                ShaderDeviceAddressKHR                     = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR,
                VideoDecodeSrcKHR                          = VK_BUFFER_USAGE_VIDEO_DECODE_SRC_BIT_KHR,
                VideoDecodeDstKHR                          = VK_BUFFER_USAGE_VIDEO_DECODE_DST_BIT_KHR,
                TransformFeedbackBufferEXT                 = VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT,
                TransformFeedbackCounterBufferEXT          = VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT,
                ConditionalRenderingEXT                    = VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT,
                AccelerationStructureBuildInputReadOnlyKHR = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR,
                AccelerationStructureStorageKHR            = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR,
                ShaderBindingTableKHR                      = VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR,
                RayTracingNV                               = VK_BUFFER_USAGE_RAY_TRACING_BIT_NV,
                VideoEncodeDstKHR                          = VK_BUFFER_USAGE_VIDEO_ENCODE_DST_BIT_KHR,
                VideoEncodeSrcKHR                          = VK_BUFFER_USAGE_VIDEO_ENCODE_SRC_BIT_KHR,
                SamplerDescriptorBufferEXT                 = VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT,
                ResourceDescriptorBufferEXT                = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT,
                PushDescriptorsDescriptorBufferEXT         = VK_BUFFER_USAGE_PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER_BIT_EXT,
                MicromapBuildInputReadOnlyEXT              = VK_BUFFER_USAGE_MICROMAP_BUILD_INPUT_READ_ONLY_BIT_EXT,
                MicromapStorageEXT                         = VK_BUFFER_USAGE_MICROMAP_STORAGE_BIT_EXT,

                VertexIndexBuffer = static_cast<uint32_t>(VertexBuffer) | static_cast<uint32_t>(IndexBuffer),
                VertexUniformBuffer = static_cast<uint32_t>(VertexBuffer) | static_cast<uint32_t>(UniformBuffer),
            };

            using VulkanFlags = vk::BufferUsageFlags;
            using VulkanBits = vk::BufferUsageFlagBits;
            using VulkanCFlags = VkBufferUsageFlags;
            using VulkanCBits = VkBufferUsageFlagBits;
        };

        struct CommandBufferUsage
        {
            enum class Bits : uint32_t
            {
                None = 0,
                OneTimeSubmit      = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
                RenderPassContinue = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
                SimultaneousUse    = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
                SimultaneousUseRenderPassContinue = static_cast<uint32_t>(SimultaneousUse) | static_cast<uint32_t>(RenderPassContinue),
            };

            using VulkanFlags = vk::CommandBufferUsageFlags;
            using VulkanBits = vk::CommandBufferUsageFlagBits;
            using VulkanCFlags = VkCommandBufferUsageFlags;
            using VulkanCBits = VkCommandBufferUsageFlagBits;
        };

        struct DebugMessageSeverity
        {
            enum class Bits : uint32_t
            {
                None = 0,
                Verbose = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
                Info    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
                Warning = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
                Error   = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                All = Verbose | Info | Warning | Error
            };

            using VulkanFlags = vk::DebugUtilsMessageSeverityFlagsEXT;
            using VulkanBits = vk::DebugUtilsMessageSeverityFlagBitsEXT;
            using VulkanCFlags = VkDebugUtilsMessageSeverityFlagsEXT;
            using VulkanCBits = VkDebugUtilsMessageSeverityFlagBitsEXT;
        };

        struct DebugMessageType
        {
            enum class Bits : uint32_t
            {
                None = 0,
                General              = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
                Validation           = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
                Performance          = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                DeviceAddressBinding = VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
                All = General | Performance | Validation | DeviceAddressBinding,
                AllNoAddressBinding = General | Performance | Validation,
            };

            using VulkanFlags = vk::DebugUtilsMessageTypeFlagsEXT;
            using VulkanBits = vk::DebugUtilsMessageTypeFlagBitsEXT;
            using VulkanCFlags = VkDebugUtilsMessageTypeFlagsEXT;
            using VulkanCBits = VkDebugUtilsMessageTypeFlagBitsEXT;
        };

        struct DescriptorSetLayoutCreate
        {
            enum class Bits : uint32_t
            {
                None = 0,
                UpdateAfterBindPool          = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
                UpdateAfterBindPoolEXT       = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT_EXT,
                PushDescriptor               = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT,
                PushDescriptorKHR            = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR,
                DescriptorBufferEXT          = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
                EmbeddedImmutableSamplersEXT = VK_DESCRIPTOR_SET_LAYOUT_CREATE_EMBEDDED_IMMUTABLE_SAMPLERS_BIT_EXT,
                IndirectBindableNV           = VK_DESCRIPTOR_SET_LAYOUT_CREATE_INDIRECT_BINDABLE_BIT_NV,
                HostOnlyPoolEXT              = VK_DESCRIPTOR_SET_LAYOUT_CREATE_HOST_ONLY_POOL_BIT_EXT,
                HostOnlyPoolVALVE            = VK_DESCRIPTOR_SET_LAYOUT_CREATE_HOST_ONLY_POOL_BIT_VALVE,
                PerStageNV                   = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PER_STAGE_BIT_NV
            };

            using VulkanFlags = vk::DescriptorSetLayoutCreateFlags;
            using VulkanBits = vk::DescriptorSetLayoutCreateFlagBits;
            using VulkanCFlags = VkDescriptorSetLayoutCreateFlags;
            using VulkanCBits = VkDescriptorSetLayoutCreateFlagBits;
        };

        struct Queue
        {
            enum class Bits : uint32_t {
                None = 0,
                Graphics       = VK_QUEUE_GRAPHICS_BIT,
                Compute        = VK_QUEUE_COMPUTE_BIT,
                Transfer       = VK_QUEUE_TRANSFER_BIT,
                SparseBinding  = VK_QUEUE_SPARSE_BINDING_BIT,
                Protected      = VK_QUEUE_PROTECTED_BIT,
                VideoDecodeKHR = VK_QUEUE_VIDEO_DECODE_BIT_KHR,
                VideoEncodeKHR = VK_QUEUE_VIDEO_ENCODE_BIT_KHR,
                OpticalFlowNV  = VK_QUEUE_OPTICAL_FLOW_BIT_NV
            };

            using VulkanFlags = vk::QueueFlags;
            using VulkanBits = vk::QueueFlagBits;
            using VulkanCFlags = VkQueueFlags;
            using VulkanCBits = VkQueueFlagBits;
        };

        struct DescriptorPoolCreate
        {
            enum class Bits : uint32_t {
                None = 0,
                FreeDescriptorSet          = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
                UpdateAfterBind            = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT,
                UpdateAfterBindEXT         = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT,
                HostOnlyEXT                = VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_EXT,
                HostOnlyVALVE              = VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_VALVE,
                AllowOverallocationSetsNV  = VK_DESCRIPTOR_POOL_CREATE_ALLOW_OVERALLOCATION_SETS_BIT_NV,
                AllowOverallocationPoolsNV = VK_DESCRIPTOR_POOL_CREATE_ALLOW_OVERALLOCATION_POOLS_BIT_NV
            };

            using VulkanFlags = vk::DescriptorPoolCreateFlags;
            using VulkanBits = vk::DescriptorPoolCreateFlagBits;
            using VulkanCFlags = VkDescriptorPoolCreateFlags;
            using VulkanCBits = VkDescriptorPoolCreateFlagBits;
        };

        struct PipelineStage
        {
            enum class Bits : uint32_t {
                None                             = VK_PIPELINE_STAGE_NONE,
                TopOfPipe                        = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                DrawIndirect                     = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
                VertexInput                      = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                VertexShader                     = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
                TessellationControlShader        = VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT,
                TessellationEvaluationShader     = VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT,
                GeometryShader                   = VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
                FragmentShader                   = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                EarlyFragmentTests               = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                LateFragmentTests                = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                ColorAttachmentOutput            = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                ComputeShader                    = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                Transfer                         = VK_PIPELINE_STAGE_TRANSFER_BIT,
                BottomOfPipe                     = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                Host                             = VK_PIPELINE_STAGE_HOST_BIT,
                AllGraphics                      = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
                AllCommands                      = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                NoneKHR                          = VK_PIPELINE_STAGE_NONE_KHR,
                TransformFeedbackEXT             = VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT,
                ConditionalRenderingEXT          = VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT,
                AccelerationStructureBuildKHR    = VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
                AccelerationStructureBuildNV     = VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_NV,
                RayTracingShaderKHR              = VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR,
                RayTracingShaderNV               = VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV,
                FragmentDensityProcessEXT        = VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT,
                FragmentShadingRateAttachmentKHR = VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
                ShadingRateImageNV               = VK_PIPELINE_STAGE_SHADING_RATE_IMAGE_BIT_NV,
                CommandPreprocessNV              = VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV,
                CommandPreprocessEXT             = VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_EXT,
                TaskShaderEXT                    = VK_PIPELINE_STAGE_TASK_SHADER_BIT_EXT,
                TaskShaderNV                     = VK_PIPELINE_STAGE_TASK_SHADER_BIT_NV,
                MeshShaderEXT                    = VK_PIPELINE_STAGE_MESH_SHADER_BIT_EXT,
                MeshShaderNV                     = VK_PIPELINE_STAGE_MESH_SHADER_BIT_NV
            };

            using VulkanFlags = vk::PipelineStageFlags;
            using VulkanBits = vk::PipelineStageFlagBits;
            using VulkanCFlags = VkPipelineStageFlags;
            using VulkanCBits = VkPipelineStageFlagBits;
        };

        struct ShaderStage
        {
            enum class Bits : uint32_t {
                None = 0,
                Vertex                 = VK_SHADER_STAGE_VERTEX_BIT,
                TessellationControl    = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
                TessellationEvaluation = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
                Geometry               = VK_SHADER_STAGE_GEOMETRY_BIT,
                Fragment               = VK_SHADER_STAGE_FRAGMENT_BIT,
                Compute                = VK_SHADER_STAGE_COMPUTE_BIT,
                AllGraphics            = VK_SHADER_STAGE_ALL_GRAPHICS,
                All                    = VK_SHADER_STAGE_ALL,
                RaygenKHR              = VK_SHADER_STAGE_RAYGEN_BIT_KHR,
                RaygenNV               = VK_SHADER_STAGE_RAYGEN_BIT_NV,
                AnyHitKHR              = VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
                AnyHitNV               = VK_SHADER_STAGE_ANY_HIT_BIT_NV,
                ClosestHitKHR          = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
                ClosestHitNV           = VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV,
                MissKHR                = VK_SHADER_STAGE_MISS_BIT_KHR,
                MissNV                 = VK_SHADER_STAGE_MISS_BIT_NV,
                IntersectionKHR        = VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
                IntersectionNV         = VK_SHADER_STAGE_INTERSECTION_BIT_NV,
                CallableKHR            = VK_SHADER_STAGE_CALLABLE_BIT_KHR,
                CallableNV             = VK_SHADER_STAGE_CALLABLE_BIT_NV,
                TaskEXT                = VK_SHADER_STAGE_TASK_BIT_EXT,
                TaskNV                 = VK_SHADER_STAGE_TASK_BIT_NV,
                MeshEXT                = VK_SHADER_STAGE_MESH_BIT_EXT,
                MeshNV                 = VK_SHADER_STAGE_MESH_BIT_NV,
                SubpassShadingHUAWEI   = VK_SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI,
                ClusterCullingHUAWEI   = VK_SHADER_STAGE_CLUSTER_CULLING_BIT_HUAWEI
            };

            using VulkanFlags = vk::ShaderStageFlags;
            using VulkanBits = vk::ShaderStageFlagBits;
            using VulkanCFlags = VkShaderStageFlags;
            using VulkanCBits = VkShaderStageFlagBits;
        };

        struct Access
        {
            enum class Bits : uint32_t {
                None                                 = VK_ACCESS_NONE,
                IndirectCommandRead                  = VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
                IndexRead                            = VK_ACCESS_INDEX_READ_BIT,
                VertexAttributeRead                  = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
                UniformRead                          = VK_ACCESS_UNIFORM_READ_BIT,
                InputAttachmentRead                  = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
                ShaderRead                           = VK_ACCESS_SHADER_READ_BIT,
                ShaderWrite                          = VK_ACCESS_SHADER_WRITE_BIT,
                ColorAttachmentRead                  = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
                ColorAttachmentWrite                 = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                DepthStencilAttachmentRead           = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
                DepthStencilAttachmentWrite          = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                TransferRead                         = VK_ACCESS_TRANSFER_READ_BIT,
                TransferWrite                        = VK_ACCESS_TRANSFER_WRITE_BIT,
                HostRead                             = VK_ACCESS_HOST_READ_BIT,
                HostWrite                            = VK_ACCESS_HOST_WRITE_BIT,
                MemoryRead                           = VK_ACCESS_MEMORY_READ_BIT,
                MemoryWrite                          = VK_ACCESS_MEMORY_WRITE_BIT,
                NoneKHR                              = VK_ACCESS_NONE_KHR,
                TransformFeedbackWriteEXT            = VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT,
                TransformFeedbackCounterReadEXT      = VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT,
                TransformFeedbackCounterWriteEXT     = VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT,
                ConditionalRenderingReadEXT          = VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT,
                ColorAttachmentReadNoncoherentEXT    = VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT,
                AccelerationStructureReadKHR         = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR,
                AccelerationStructureReadNV          = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV,
                AccelerationStructureWriteKHR        = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR,
                AccelerationStructureWriteNV         = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV,
                FragmentDensityMapReadEXT            = VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT,
                FragmentShadingRateAttachmentReadKHR = VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR,
                ShadingRateImageReadNV               = VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV,
                CommandPreprocessReadNV              = VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV,
                CommandPreprocessReadEXT             = VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_EXT,
                CommandPreprocessWriteNV             = VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV,
                CommandPreprocessWriteEXT            = VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_EXT
            };

            using VulkanFlags = vk::AccessFlags;
            using VulkanBits = vk::AccessFlagBits;
            using VulkanCFlags = VkAccessFlags;
            using VulkanCBits = VkAccessFlagBits;
        };

        struct ImageUsage
        {
            enum class Bits : uint32_t {
                None = 0,
                TransferSrc                        = VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                TransferDst                        = VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                Sampled                            = VK_IMAGE_USAGE_SAMPLED_BIT,
                Storage                            = VK_IMAGE_USAGE_STORAGE_BIT,
                ColorAttachment                    = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                DepthStencilAttachment             = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                TransientAttachment                = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
                InputAttachment                    = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                HostTransfer                       = VK_IMAGE_USAGE_HOST_TRANSFER_BIT,
                HostTransferEXT                    = VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT,
                VideoDecodeDstKHR                  = VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR,
                VideoDecodeSrcKHR                  = VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR,
                VideoDecodeDpbKHR                  = VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR,
                FragmentDensityMapEXT              = VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT,
                FragmentShadingRateAttachmentKHR   = VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
                ShadingRateImageNV                 = VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV,
                VideoEncodeDstKHR                  = VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR,
                VideoEncodeSrcKHR                  = VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR,
                VideoEncodeDpbKHR                  = VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR,
                AttachmentFeedbackLoopEXT          = VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT,
                InvocationMaskHUAWEI               = VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI,
                SampleWeightQCOM                   = VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM,
                SampleBlockMatchQCOM               = VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM,
                VideoEncodeQuantizationDeltaMapKHR = VK_IMAGE_USAGE_VIDEO_ENCODE_QUANTIZATION_DELTA_MAP_BIT_KHR,
                VideoEncodeEmphasisMapKHR          = VK_IMAGE_USAGE_VIDEO_ENCODE_EMPHASIS_MAP_BIT_KHR,

                TransferSrcDst = static_cast<uint32_t>(TransferSrc) | static_cast<uint32_t>(TransferDst),
                ColorAttachmentSampled = static_cast<uint32_t>(ColorAttachment) | static_cast<uint32_t>(Sampled),
                DepthStencilAttachmentSampled = static_cast<uint32_t>(DepthStencilAttachment) | static_cast<uint32_t>(Sampled),
                StorageSampled = static_cast<uint32_t>(Storage) | static_cast<uint32_t>(Sampled),
            };

            using VulkanFlags = vk::ImageUsageFlags;
            using VulkanBits = vk::ImageUsageFlagBits;
            using VulkanCFlags = VkImageUsageFlags;
            using VulkanCBits = VkImageUsageFlagBits;
        };

        struct ImageAspect
        {
            enum class Bits : uint32_t {
                None            = VK_IMAGE_ASPECT_NONE,
                Color           = VK_IMAGE_ASPECT_COLOR_BIT,
                Depth           = VK_IMAGE_ASPECT_DEPTH_BIT,
                Stencil         = VK_IMAGE_ASPECT_STENCIL_BIT,
                Metadata        = VK_IMAGE_ASPECT_METADATA_BIT,
                Plane0          = VK_IMAGE_ASPECT_PLANE_0_BIT,
                Plane0KHR       = VK_IMAGE_ASPECT_PLANE_0_BIT_KHR,
                Plane1          = VK_IMAGE_ASPECT_PLANE_1_BIT,
                Plane1KHR       = VK_IMAGE_ASPECT_PLANE_1_BIT_KHR,
                Plane2          = VK_IMAGE_ASPECT_PLANE_2_BIT,
                Plane2KHR       = VK_IMAGE_ASPECT_PLANE_2_BIT_KHR,
                NoneKHR         = VK_IMAGE_ASPECT_NONE_KHR,
                MemoryPlane0EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT,
                MemoryPlane1EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT,
                MemoryPlane2EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT,
                MemoryPlane3EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT,

                DepthStencil = static_cast<uint32_t>(Depth) | static_cast<uint32_t>(Stencil),
                AllPlanes = static_cast<uint32_t>(Plane0) | static_cast<uint32_t>(Plane1) | static_cast<uint32_t>(Plane2),
            };

            using VulkanFlags = vk::ImageAspectFlags;
            using VulkanBits = vk::ImageAspectFlagBits;
            using VulkanCFlags = VkImageAspectFlags;
            using VulkanCBits = VkImageAspectFlagBits;
        };

        struct FormatFeature
        {
            enum class Bits : uint32_t {
                None = 0,
                SampledImage                                               = VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
                StorageImage                                               = VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT,
                StorageImageAtomic                                         = VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT,
                UniformTexelBuffer                                         = VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT,
                StorageTexelBuffer                                         = VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT,
                StorageTexelBufferAtomic                                   = VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT,
                VertexBuffer                                               = VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT,
                ColorAttachment                                            = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,
                ColorAttachmentBlend                                       = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT,
                DepthStencilAttachment                                     = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
                BlitSrc                                                    = VK_FORMAT_FEATURE_BLIT_SRC_BIT,
                BlitDst                                                    = VK_FORMAT_FEATURE_BLIT_DST_BIT,
                SampledImageFilterLinear                                   = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT,
                TransferSrc                                                = VK_FORMAT_FEATURE_TRANSFER_SRC_BIT,
                TransferSrcKHR                                             = VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR,
                TransferDst                                                = VK_FORMAT_FEATURE_TRANSFER_DST_BIT,
                TransferDstKHR                                             = VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR,
                MidpointChromaSamples                                      = VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT,
                MidpointChromaSamplesKHR                                   = VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT_KHR,
                SampledImageYcbcrConversionLinearFilter                    = VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT,
                SampledImageYcbcrConversionLinearFilterKHR                 = VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT_KHR,
                SampledImageYcbcrConversionSeparateReconstructionFilter    = VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT,
                SampledImageYcbcrConversionSeparateReconstructionFilterKHR = VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT_KHR,
                SampledImageYcbcrConversionChromaReconstructionExplicit    = VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT,
                SampledImageYcbcrConversionChromaReconstructionExplicitKHR = VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT_KHR,
                SampledImageYcbcrConversionChromaReconstructionExplicitForceable =
                VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT,
                SampledImageYcbcrConversionChromaReconstructionExplicitForceableKHR =
                VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT_KHR,
                Disjoint                             = VK_FORMAT_FEATURE_DISJOINT_BIT,
                DisjointKHR                          = VK_FORMAT_FEATURE_DISJOINT_BIT_KHR,
                CositedChromaSamples                 = VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT,
                CositedChromaSamplesKHR              = VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT_KHR,
                SampledImageFilterMinmax             = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT,
                SampledImageFilterMinmaxEXT          = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT,
                VideoDecodeOutputKHR                 = VK_FORMAT_FEATURE_VIDEO_DECODE_OUTPUT_BIT_KHR,
                VideoDecodeDpbKHR                    = VK_FORMAT_FEATURE_VIDEO_DECODE_DPB_BIT_KHR,
                AccelerationStructureVertexBufferKHR = VK_FORMAT_FEATURE_ACCELERATION_STRUCTURE_VERTEX_BUFFER_BIT_KHR,
                SampledImageFilterCubicEXT           = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_EXT,
                SampledImageFilterCubicIMG           = VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG,
                FragmentDensityMapEXT                = VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT,
                FragmentShadingRateAttachmentKHR     = VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
                VideoEncodeInputKHR                  = VK_FORMAT_FEATURE_VIDEO_ENCODE_INPUT_BIT_KHR,
                VideoEncodeDpbKHR                    = VK_FORMAT_FEATURE_VIDEO_ENCODE_DPB_BIT_KHR
            };

            using VulkanFlags = vk::FormatFeatureFlags;
            using VulkanBits = vk::FormatFeatureFlagBits;
            using VulkanCFlags = VkFormatFeatureFlags;
            using VulkanCBits = VkFormatFeatureFlagBits;
        };

        struct FenceCreate
        {
            enum class Bits : uint32_t {
                None = 0,
                Signaled = VK_FENCE_CREATE_SIGNALED_BIT,
            };

            using VulkanFlags = vk::FenceCreateFlags;
            using VulkanBits = vk::FenceCreateFlagBits;
            using VulkanCFlags = VkFenceCreateFlags;
            using VulkanCBits = VkFenceCreateFlagBits;
        };

        struct CommandPoolCreate
        {
            enum class Bits : uint32_t {
                None = 0,
                Transient          = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
                ResetCommandBuffer = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                Protected          = VK_COMMAND_POOL_CREATE_PROTECTED_BIT
            };

            using VulkanFlags = vk::CommandPoolCreateFlags;
            using VulkanBits = vk::CommandPoolCreateFlagBits;
            using VulkanCFlags = VkCommandPoolCreateFlags;
            using VulkanCBits = VkCommandPoolCreateFlagBits;
        };

        struct CommandPoolReset
        {
            enum class Bits : uint32_t {
                None = 0,
                ReleaseResources = VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT,
            };

            using VulkanFlags = vk::CommandPoolResetFlags;
            using VulkanBits = vk::CommandPoolResetFlagBits;
            using VulkanCFlags = VkCommandPoolResetFlags;
            using VulkanCBits = VkCommandPoolResetFlagBits;
        };

        struct DebugUtilsMessengerCallbackData
        {
            enum class Bits : uint32_t {
                None = 0,
            };

            using VulkanFlags = vk::DebugUtilsMessengerCallbackDataFlagsEXT;
            using VulkanBits = vk::DebugUtilsMessengerCallbackDataFlagBitsEXT;
            using VulkanCFlags = VkDebugUtilsMessengerCallbackDataFlagsEXT;
            using VulkanCBits = void;
        };

        struct SurfaceTransform
        {
            enum class Bits : uint32_t {
                None = 0,
                Identity                  = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                Rotate90                  = VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR,
                Rotate180                 = VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR,
                Rotate270                 = VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR,
                HorizontalMirror          = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR,
                HorizontalMirrorRotate90  = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR,
                HorizontalMirrorRotate180 = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR,
                HorizontalMirrorRotate270 = VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR,
                Inherit                   = VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR
            };

            using VulkanFlags = vk::SurfaceTransformFlagsKHR;
            using VulkanBits = vk::SurfaceTransformFlagBitsKHR;
            using VulkanCFlags = VkSurfaceTransformFlagsKHR;
            using VulkanCBits = VkSurfaceTransformFlagBitsKHR;
        };

        struct SemaphoreCreate
        {
            enum class Bits : uint32_t {
                None = 0,
            };

            using VulkanFlags = vk::SemaphoreCreateFlags;
            using VulkanBits = vk::SemaphoreCreateFlagBits;
            using VulkanCFlags = VkSemaphoreCreateFlags;
            using VulkanCBits = void;
        };

        struct SampleCount
        {
            enum class Bits : uint32_t {
                None = 0,
                SC1  = VK_SAMPLE_COUNT_1_BIT,
                SC2  = VK_SAMPLE_COUNT_2_BIT,
                SC4  = VK_SAMPLE_COUNT_4_BIT,
                SC8  = VK_SAMPLE_COUNT_8_BIT,
                SC16 = VK_SAMPLE_COUNT_16_BIT,
                SC32 = VK_SAMPLE_COUNT_32_BIT,
                SC64 = VK_SAMPLE_COUNT_64_BIT,
                SampleCountAll = SC1 | SC2 | SC4 | SC8 | SC16 | SC32 | SC64
            };

            using VulkanFlags = vk::SampleCountFlags;
            using VulkanBits = vk::SampleCountFlagBits;
            using VulkanCFlags = VkSampleCountFlags;
            using VulkanCBits = VkSampleCountFlagBits;
        };

        struct AttachmentCreate
        {
            enum class Bits : uint32_t {
                None = 0,
                MayAlias = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT,
            };

            using VulkanFlags = vk::AttachmentDescriptionFlags;
            using VulkanBits = vk::AttachmentDescriptionFlagBits;
            using VulkanCFlags = VkAttachmentDescriptionFlags;
            using VulkanCBits = VkAttachmentDescriptionFlagBits;
        };

        struct Dependency
        {
            enum class Bits : uint32_t {
                None = 0,
                ByRegion                                    = VK_DEPENDENCY_BY_REGION_BIT,
                DeviceGroup                                 = VK_DEPENDENCY_DEVICE_GROUP_BIT,
                DeviceGroupKHR                              = VK_DEPENDENCY_DEVICE_GROUP_BIT_KHR,
                ViewLocal                                   = VK_DEPENDENCY_VIEW_LOCAL_BIT,
                ViewLocalKHR                                = VK_DEPENDENCY_VIEW_LOCAL_BIT_KHR,
                FeedbackLoopEXT                             = VK_DEPENDENCY_FEEDBACK_LOOP_BIT_EXT,
                QueueFamilyOwnershipTransferUseAllStagesKHR = VK_DEPENDENCY_QUEUE_FAMILY_OWNERSHIP_TRANSFER_USE_ALL_STAGES_BIT_KHR
            };

            using VulkanFlags = vk::DependencyFlags;
            using VulkanBits = vk::DependencyFlagBits;
            using VulkanCFlags = VkDependencyFlags;
            using VulkanCBits = VkDependencyFlagBits;
        };

        struct CompositeAlpha
        {
            enum class Bits : uint32_t {
                None = 0,
                Opaque         = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                PreMultiplied  = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
                PostMultiplied = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
                Inherit        = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
            };

            using VulkanFlags = vk::CompositeAlphaFlagsKHR;
            using VulkanBits = vk::CompositeAlphaFlagBitsKHR;
            using VulkanCFlags = VkCompositeAlphaFlagsKHR;
            using VulkanCBits = VkCompositeAlphaFlagBitsKHR;
        };

        struct PipelineLayoutCreate
        {
            enum class Bits : uint32_t {
                None = 0,
                IndependentSetsEXT = VK_PIPELINE_LAYOUT_CREATE_INDEPENDENT_SETS_BIT_EXT,
            };

            using VulkanFlags = vk::PipelineLayoutCreateFlags;
            using VulkanBits = vk::PipelineLayoutCreateFlagBits;
            using VulkanCFlags = VkPipelineLayoutCreateFlags;
            using VulkanCBits = VkPipelineLayoutCreateFlagBits;
        };

        struct PipelineCreate
        {
            enum class Bits : uint32_t {
                None = 0,
                DisableOptimization                                                = VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT,
                AllowDerivatives                                                   = VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT,
                Derivative                                                         = VK_PIPELINE_CREATE_DERIVATIVE_BIT,
                ViewIndexFromDeviceIndex                                           = VK_PIPELINE_CREATE_VIEW_INDEX_FROM_DEVICE_INDEX_BIT,
                ViewIndexFromDeviceIndexKHR                                        = VK_PIPELINE_CREATE_VIEW_INDEX_FROM_DEVICE_INDEX_BIT_KHR,
                DispatchBase                                                       = VK_PIPELINE_CREATE_DISPATCH_BASE_BIT,
                DispatchBaseKHR                                                    = VK_PIPELINE_CREATE_DISPATCH_BASE_KHR,
                FailOnPipelineCompileRequired                                      = VK_PIPELINE_CREATE_FAIL_ON_PIPELINE_COMPILE_REQUIRED_BIT,
                FailOnPipelineCompileRequiredEXT                                   = VK_PIPELINE_CREATE_FAIL_ON_PIPELINE_COMPILE_REQUIRED_BIT_EXT,
                EarlyReturnOnFailure                                               = VK_PIPELINE_CREATE_EARLY_RETURN_ON_FAILURE_BIT,
                EarlyReturnOnFailureEXT                                            = VK_PIPELINE_CREATE_EARLY_RETURN_ON_FAILURE_BIT_EXT,
                NoProtectedAccess                                                  = VK_PIPELINE_CREATE_NO_PROTECTED_ACCESS_BIT,
                NoProtectedAccessEXT                                               = VK_PIPELINE_CREATE_NO_PROTECTED_ACCESS_BIT_EXT,
                ProtectedAccessOnly                                                = VK_PIPELINE_CREATE_PROTECTED_ACCESS_ONLY_BIT,
                ProtectedAccessOnlyEXT                                             = VK_PIPELINE_CREATE_PROTECTED_ACCESS_ONLY_BIT_EXT,
                RayTracingNoNullAnyHitShadersKHR                                   = VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_ANY_HIT_SHADERS_BIT_KHR,
                RayTracingNoNullClosestHitShadersKHR                               = VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_CLOSEST_HIT_SHADERS_BIT_KHR,
                RayTracingNoNullMissShadersKHR                                     = VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_MISS_SHADERS_BIT_KHR,
                RayTracingNoNullIntersectionShadersKHR                             = VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_INTERSECTION_SHADERS_BIT_KHR,
                RayTracingSkipTrianglesKHR                                         = VK_PIPELINE_CREATE_RAY_TRACING_SKIP_TRIANGLES_BIT_KHR,
                RayTracingSkipAabbsKHR                                             = VK_PIPELINE_CREATE_RAY_TRACING_SKIP_AABBS_BIT_KHR,
                RayTracingShaderGroupHandleCaptureReplayKHR                        = VK_PIPELINE_CREATE_RAY_TRACING_SHADER_GROUP_HANDLE_CAPTURE_REPLAY_BIT_KHR,
                DeferCompileNV                                                     = VK_PIPELINE_CREATE_DEFER_COMPILE_BIT_NV,
                RenderingFragmentDensityMapAttachmentEXT                           = VK_PIPELINE_CREATE_RENDERING_FRAGMENT_DENSITY_MAP_ATTACHMENT_BIT_EXT,
                VkPipelineRasterizationStateCreateFragmentDensityMapAttachmentEXT  = VK_PIPELINE_RASTERIZATION_STATE_CREATE_FRAGMENT_DENSITY_MAP_ATTACHMENT_BIT_EXT,
                RenderingFragmentShadingRateAttachmentKHR                          = VK_PIPELINE_CREATE_RENDERING_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
                VkPipelineRasterizationStateCreateFragmentShadingRateAttachmentKHR = VK_PIPELINE_RASTERIZATION_STATE_CREATE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
                CaptureStatisticsKHR                                               = VK_PIPELINE_CREATE_CAPTURE_STATISTICS_BIT_KHR,
                CaptureInternalRepresentationsKHR                                  = VK_PIPELINE_CREATE_CAPTURE_INTERNAL_REPRESENTATIONS_BIT_KHR,
                IndirectBindableNV                                                 = VK_PIPELINE_CREATE_INDIRECT_BINDABLE_BIT_NV,
                LibraryKHR                                                         = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR,
                DescriptorBufferEXT                                                = VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
                RetainLinkTimeOptimizationInfoEXT                                  = VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT,
                LinkTimeOptimizationEXT                                            = VK_PIPELINE_CREATE_LINK_TIME_OPTIMIZATION_BIT_EXT,
                RayTracingAllowMotionNV                                            = VK_PIPELINE_CREATE_RAY_TRACING_ALLOW_MOTION_BIT_NV,
                ColorAttachmentFeedbackLoopEXT                                     = VK_PIPELINE_CREATE_COLOR_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT,
                DepthStencilAttachmentFeedbackLoopEXT                              = VK_PIPELINE_CREATE_DEPTH_STENCIL_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT,
                RayTracingOpacityMicromapEXT                                       = VK_PIPELINE_CREATE_RAY_TRACING_OPACITY_MICROMAP_BIT_EXT,
            };

            using VulkanFlags = vk::PipelineCreateFlags;
            using VulkanBits = vk::PipelineCreateFlagBits;
            using VulkanCFlags = VkPipelineCreateFlags;
            using VulkanCBits = VkPipelineCreateFlagBits;
        };

        struct PipelineShaderStageCreate
        {
            enum class Bits : uint32_t {
                None = 0,
                AllowVaryingSubgroupSize    = VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT,
                AllowVaryingSubgroupSizeEXT = VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT_EXT,
                RequireFullSubgroups        = VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT,
                RequireFullSubgroupsEXT     = VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT_EXT
            };

            using VulkanFlags = vk::PipelineShaderStageCreateFlags;
            using VulkanBits = vk::PipelineShaderStageCreateFlagBits;
            using VulkanCFlags = VkPipelineShaderStageCreateFlags;
            using VulkanCBits = VkPipelineShaderStageCreateFlagBits;
        };

        struct PipelineVertexInputStateCreate
        {
            enum class Bits : uint32_t {
                None = 0,
            };

            using VulkanFlags = vk::PipelineVertexInputStateCreateFlags;
            using VulkanBits = vk::PipelineVertexInputStateCreateFlagBits;
            using VulkanCFlags = VkPipelineVertexInputStateCreateFlags;
            using VulkanCBits = void;
        };

        struct PipelineInputAssemblyStateCreate
        {
            enum class Bits : uint32_t {
                None = 0,
            };

            using VulkanFlags = vk::PipelineInputAssemblyStateCreateFlags;
            using VulkanBits = vk::PipelineInputAssemblyStateCreateFlagBits;
            using VulkanCFlags = VkPipelineInputAssemblyStateCreateFlags;
            using VulkanCBits = void;
        };

        struct PipelineTessellationStateCreate
        {
            enum class Bits : uint32_t {
                None = 0,
            };
            using VulkanFlags = vk::PipelineTessellationStateCreateFlags;
            using VulkanBits = vk::PipelineTessellationStateCreateFlagBits;
            using VulkanCFlags = VkPipelineTessellationStateCreateFlags;
            using VulkanCBits = void;
        };

        struct PipelineViewportStateCreate
        {
            enum class Bits : uint32_t {
                None = 0,
            };
            using VulkanFlags = vk::PipelineViewportStateCreateFlags;
            using VulkanBits = vk::PipelineViewportStateCreateFlagBits;
            using VulkanCFlags = VkPipelineViewportStateCreateFlags;
            using VulkanCBits = void;
        };

        struct PipelineRasterizationStateCreate
        {
            enum class Bits : uint32_t {
                None = 0,
            };
            using VulkanFlags = vk::PipelineRasterizationStateCreateFlags;
            using VulkanBits = vk::PipelineRasterizationStateCreateFlagBits;
            using VulkanCFlags = VkPipelineRasterizationStateCreateFlags;
            using VulkanCBits = void;
        };

        struct CullMode
        {
            enum class Bits : uint32_t {
                None         = VK_CULL_MODE_NONE,
                Front        = VK_CULL_MODE_FRONT_BIT,
                Back         = VK_CULL_MODE_BACK_BIT,
                FrontAndBack = VK_CULL_MODE_FRONT_AND_BACK
            };
            using VulkanFlags = vk::CullModeFlags;
            using VulkanBits = vk::CullModeFlagBits;
            using VulkanCFlags = VkCullModeFlags;
            using VulkanCBits = VkCullModeFlagBits;
        };

        struct PipelineMultisampleStateCreate
        {
            enum class Bits : uint32_t {
                None = 0
            };
            using VulkanFlags = vk::PipelineMultisampleStateCreateFlags;
            using VulkanBits = vk::PipelineMultisampleStateCreateFlagBits;
            using VulkanCFlags = VkPipelineMultisampleStateCreateFlags;
            using VulkanCBits = void;
        };

        struct PipelineDepthStencilStateCreate
        {
            enum class Bits : uint32_t {
                None = 0
            };
            using VulkanFlags = vk::PipelineDepthStencilStateCreateFlags;
            using VulkanBits = vk::PipelineDepthStencilStateCreateFlagBits;
            using VulkanCFlags = VkPipelineDepthStencilStateCreateFlags;
            using VulkanCBits = VkPipelineDepthStencilStateCreateFlagBits;
        };

        struct PipelineColorBlendStateCreate
        {
            enum class Bits : uint32_t {
                None = 0,
                RasterizationOrderAttachmentAccessEXT = VK_PIPELINE_COLOR_BLEND_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_BIT_EXT,
                RasterizationOrderAttachmentAccessARM = VK_PIPELINE_COLOR_BLEND_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_BIT_ARM
            };
            using VulkanFlags = vk::PipelineColorBlendStateCreateFlags;
            using VulkanBits = vk::PipelineColorBlendStateCreateFlagBits;
            using VulkanCFlags = VkPipelineColorBlendStateCreateFlags;
            using VulkanCBits = VkPipelineColorBlendStateCreateFlagBits;
        };

        struct PipelineDynamicStateCreate
        {
            enum class Bits : uint32_t {
                None = 0,
            };
            using VulkanFlags = vk::PipelineDynamicStateCreateFlags;
            using VulkanBits = vk::PipelineDynamicStateCreateFlagBits;
            using VulkanCFlags = VkPipelineDynamicStateCreateFlags;
            using VulkanCBits = void;
        };

        struct ColorComponent
        {
            enum class Bits : uint32_t {
                None = 0,
                R = VK_COLOR_COMPONENT_R_BIT,
                G = VK_COLOR_COMPONENT_G_BIT,
                B = VK_COLOR_COMPONENT_B_BIT,
                A = VK_COLOR_COMPONENT_A_BIT
            };
            using VulkanFlags = vk::ColorComponentFlags;
            using VulkanBits = vk::ColorComponentFlagBits;
            using VulkanCFlags = VkColorComponentFlags;
            using VulkanCBits = VkColorComponentFlagBits;
        };

        struct DescriptorPoolReset
        {
            enum class Bits : uint32_t {
                None = 0,
            };

            using VulkanFlags = vk::DescriptorPoolResetFlags;
            using VulkanBits = vk::DescriptorPoolResetFlagBits;
            using VulkanCFlags = VkDescriptorPoolResetFlags;
            using VulkanCBits = void;
        };

        struct QueryControl
        {
            enum class Bits : uint32_t {
                None = 0,
                Precise = VK_QUERY_CONTROL_PRECISE_BIT
            };

            using VulkanFlags = vk::QueryControlFlags;
            using VulkanBits = vk::QueryControlFlagBits;
            using VulkanCFlags = VkQueryControlFlags;
            using VulkanCBits = VkQueryControlFlagBits;
        };

        struct QueryPipelineStatistic
        {
            enum class Bits : uint32_t {
                None = 0,
                InputAssemblyVertices                   = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT,
                InputAssemblyPrimitives                 = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT,
                VertexShaderInvocations                 = VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT,
                GeometryShaderInvocations               = VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT,
                GeometryShaderPrimitives                = VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT,
                ClippingInvocations                     = VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT,
                ClippingPrimitives                      = VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT,
                FragmentShaderInvocations               = VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT,
                TessellationControlShaderPatches        = VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT,
                TessellationEvaluationShaderInvocations = VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT,
                ComputeShaderInvocations                = VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT,
                TaskShaderInvocationsEXT                = VK_QUERY_PIPELINE_STATISTIC_TASK_SHADER_INVOCATIONS_BIT_EXT,
                MeshShaderInvocationsEXT                = VK_QUERY_PIPELINE_STATISTIC_MESH_SHADER_INVOCATIONS_BIT_EXT,
                ClusterCullingShaderInvocationsHUAWEI   = VK_QUERY_PIPELINE_STATISTIC_CLUSTER_CULLING_SHADER_INVOCATIONS_BIT_HUAWEI
            };

            using VulkanFlags = vk::QueryPipelineStatisticFlags;
            using VulkanBits = vk::QueryPipelineStatisticFlagBits;
            using VulkanCFlags = VkQueryPipelineStatisticFlags;
            using VulkanCBits = VkQueryPipelineStatisticFlagBits;
        };

        struct SamplerCreate
        {
            enum class Bits : uint32_t {
                None = 0,
                SubsampledEXT                     = VK_SAMPLER_CREATE_SUBSAMPLED_BIT_EXT,
                SubsampledCoarseReconstructionEXT = VK_SAMPLER_CREATE_SUBSAMPLED_COARSE_RECONSTRUCTION_BIT_EXT,
                DescriptorBufferCaptureReplayEXT  = VK_SAMPLER_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT,
                NonSeamlessCubeMapEXT             = VK_SAMPLER_CREATE_NON_SEAMLESS_CUBE_MAP_BIT_EXT,
                ImageProcessingQCOM               = VK_SAMPLER_CREATE_IMAGE_PROCESSING_BIT_QCOM
            };

            using VulkanFlags = vk::SamplerCreateFlags;
            using VulkanBits = vk::SamplerCreateFlagBits;
            using VulkanCFlags = VkSamplerCreateFlags;
            using VulkanCBits = VkSamplerCreateFlagBits;
        };

        struct CommandBufferReset
        {
            enum class Bits : uint32_t {
                None = 0,
                ReleaseResources = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT
            };

            using VulkanFlags = vk::CommandBufferResetFlags;
            using VulkanBits = vk::CommandBufferResetFlagBits;
            using VulkanCFlags = VkCommandBufferResetFlags;
            using VulkanCBits = VkCommandBufferResetFlagBits;
        };

        struct BufferViewCreate
        {
            enum class Bits : uint32_t {
                None = 0,
            };

            using VulkanFlags = vk::BufferViewCreateFlags;
            using VulkanBits = vk::BufferViewCreateFlagBits;
            using VulkanCFlags = VkBufferViewCreateFlags;
            using VulkanCBits = void;
        };

        struct MemoryMap
        {
            enum class Bits : uint32_t {
                None = 0,
                PlacedEXT = VK_MEMORY_MAP_PLACED_BIT_EXT,
            };

            using VulkanFlags = vk::MemoryMapFlags;
            using VulkanBits = vk::MemoryMapFlagBits;
            using VulkanCFlags = VkMemoryMapFlags;
            using VulkanCBits = VkMemoryMapFlagBits;
        };

        struct ImageCreate
        {
            enum class Bits : uint32_t {
                None = 0,
                SparseBinding = VK_IMAGE_CREATE_SPARSE_BINDING_BIT,
                SparseResidency = VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT,
                SparseAliased = VK_IMAGE_CREATE_SPARSE_ALIASED_BIT,
                MutableFormat = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
                CubeCompatible = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
                Alias = VK_IMAGE_CREATE_ALIAS_BIT,
                AliasKHR = VK_IMAGE_CREATE_ALIAS_BIT_KHR,
                SplitInstanceBindRegions = VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT,
                SplitInstanceBindRegionsKHR = VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR,
                Array2DCompatible = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT,
                Array2DCompatibleKHR = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT_KHR,
                BlockTexelViewCompatible = VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT,
                BlockTexelViewCompatibleKHR = VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT_KHR,
                ExtendedUsage = VK_IMAGE_CREATE_EXTENDED_USAGE_BIT,
                ExtendedUsageKHR = VK_IMAGE_CREATE_EXTENDED_USAGE_BIT_KHR,
                Protected = VK_IMAGE_CREATE_PROTECTED_BIT,
                Disjoint = VK_IMAGE_CREATE_DISJOINT_BIT,
                DisjointKHR = VK_IMAGE_CREATE_DISJOINT_BIT_KHR,
                CornerSampledNV = VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV,
                SampleLocationsCompatibleDepthEXT = VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT,
                SubsampledEXT = VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT,
                DescriptorBufferCaptureReplayEXT = VK_IMAGE_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT,
                MultisampledRenderToSingleSampledEXT = VK_IMAGE_CREATE_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_BIT_EXT,
                View2DCompatibleEXT = VK_IMAGE_CREATE_2D_VIEW_COMPATIBLE_BIT_EXT,
                FragmentDensityMapOffsetQCOM = VK_IMAGE_CREATE_FRAGMENT_DENSITY_MAP_OFFSET_BIT_QCOM,
                VideoProfileIndependentKHR = VK_IMAGE_CREATE_VIDEO_PROFILE_INDEPENDENT_BIT_KHR
            };

            using VulkanFlags = vk::ImageCreateFlags;
            using VulkanBits = vk::ImageCreateFlagBits;
            using VulkanCFlags = VkImageCreateFlags;
            using VulkanCBits = VkImageCreateFlagBits;
        };

        struct SurfaceCreate
        {
            enum class Bits : uint32_t {
                None = 0,
            };

            using VulkanFlags = void;
            using VulkanBits = void;
            using VulkanCFlags = void;
            using VulkanCBits = void;
        };

        struct DescriptorBinding {
            enum class Bits : uint32_t {
                None = 0,
                UpdateAfterBind          = VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT,
                UpdateUnusedWhilePending = VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT,
                PartiallyBound           = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT,
                VariableDescriptorCount  = VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT
            };

            using VulkanFlags = vk::DescriptorBindingFlags;
            using VulkanBits = vk::DescriptorBindingFlagBits;
            using VulkanCFlags = VkDescriptorBindingFlags;
            using VulkanCBits = VkDescriptorBindingFlagBits;
        };

        struct DeviceCreate {
            enum class Bits : uint32_t {
                None = 0
            };

            using VulkanFlags = vk::DeviceCreateFlags;
            using VulkanBits = vk::DeviceCreateFlagBits;
            using VulkanCFlags = VkDeviceCreateFlags;
            using VulkanCBits = void;
        };
    }

    // Type aliases using FlagsBase with traits
    using MemoryProperty = FlagsBase<Traits::MemoryProperty>;
    using MemoryHeap = FlagsBase<Traits::MemoryHeap>;
    using BufferUsage = FlagsBase<Traits::BufferUsage>;
    using CommandBufferUsage = FlagsBase<Traits::CommandBufferUsage>;
    using DebugMessageSeverity = FlagsBase<Traits::DebugMessageSeverity>;
    using DebugMessageType = FlagsBase<Traits::DebugMessageType>;
    using DescriptorSetLayoutCreate = FlagsBase<Traits::DescriptorSetLayoutCreate>;
    using Queue = FlagsBase<Traits::Queue>;
    using DescriptorPoolCreate = FlagsBase<Traits::DescriptorPoolCreate>;
    using PipelineStage = FlagsBase<Traits::PipelineStage>;
    using ShaderStage = FlagsBase<Traits::ShaderStage>;
    using Access = FlagsBase<Traits::Access>;
    using ImageUsage = FlagsBase<Traits::ImageUsage>;
    using ImageAspect = FlagsBase<Traits::ImageAspect>;
    using FormatFeature = FlagsBase<Traits::FormatFeature>;
    using FenceCreate = FlagsBase<Traits::FenceCreate>;
    using CommandPoolCreate = FlagsBase<Traits::CommandPoolCreate>;
    using CommandPoolReset = FlagsBase<Traits::CommandPoolReset>;
    using SurfaceTransform = FlagsBase<Traits::SurfaceTransform>;
    using SemaphoreCreate = FlagsBase<Traits::SemaphoreCreate>;
    using SampleCount = FlagsBase<Traits::SampleCount>;
    using AttachmentCreate = FlagsBase<Traits::AttachmentCreate>;
    using Dependency = FlagsBase<Traits::Dependency>;
    using CompositeAlpha = FlagsBase<Traits::CompositeAlpha>;
    using PipelineLayoutCreate = FlagsBase<Traits::PipelineLayoutCreate>;
    using PipelineCreate = FlagsBase<Traits::PipelineCreate>;
    using PipelineShaderStageCreate = FlagsBase<Traits::PipelineShaderStageCreate>;
    using PipelineVertexInputStateCreate = FlagsBase<Traits::PipelineVertexInputStateCreate>;
    using PipelineInputAssemblyStateCreate = FlagsBase<Traits::PipelineInputAssemblyStateCreate>;
    using PipelineTessellationStateCreate = FlagsBase<Traits::PipelineTessellationStateCreate>;
    using PipelineViewportStateCreate = FlagsBase<Traits::PipelineViewportStateCreate>;
    using PipelineRasterizationStateCreate = FlagsBase<Traits::PipelineRasterizationStateCreate>;
    using CullMode = FlagsBase<Traits::CullMode>;
    using PipelineMultisampleStateCreate = FlagsBase<Traits::PipelineMultisampleStateCreate>;
    using PipelineDepthStencilStateCreate = FlagsBase<Traits::PipelineDepthStencilStateCreate>;
    using PipelineColorBlendStateCreate = FlagsBase<Traits::PipelineColorBlendStateCreate>;
    using PipelineDynamicStateCreate = FlagsBase<Traits::PipelineDynamicStateCreate>;
    using ColorComponent = FlagsBase<Traits::ColorComponent>;
    using DescriptorPoolCreate = FlagsBase<Traits::DescriptorPoolCreate>;
    using DescriptorPoolReset = FlagsBase<Traits::DescriptorPoolReset>;
    using QueryControl = FlagsBase<Traits::QueryControl>;
    using QueryPipelineStatistic = FlagsBase<Traits::QueryPipelineStatistic>;
    using SamplerCreate = FlagsBase<Traits::SamplerCreate>;
    using CommandBufferReset = FlagsBase<Traits::CommandBufferReset>;
    using BufferViewCreate = FlagsBase<Traits::BufferViewCreate>;
    using MemoryMap = FlagsBase<Traits::MemoryMap>;
    using ImageCreate = FlagsBase<Traits::ImageCreate>;
    using SurfaceCreate = FlagsBase<Traits::SurfaceCreate>;
    using DescriptorBinding = FlagsBase<Traits::DescriptorBinding>;
    using DeviceCreate = FlagsBase<Traits::DeviceCreate>;

    // DebugUtils namespace
    namespace DebugUtils {
        using MessengerCallbackData = FlagsBase<Traits::DebugUtilsMessengerCallbackData>;
    }

    // BitTraits specializations
    template<> struct BitTraits<MemoryProperty::Bits> { using ParentType = MemoryProperty; };
    template<> struct BitTraits<MemoryHeap::Bits> { using ParentType = MemoryHeap; };
    template<> struct BitTraits<BufferUsage::Bits> { using ParentType = BufferUsage; };
    template<> struct BitTraits<CommandBufferUsage::Bits> { using ParentType = CommandBufferUsage; };
    template<> struct BitTraits<DebugMessageSeverity::Bits> { using ParentType = DebugMessageSeverity; };
    template<> struct BitTraits<DebugMessageType::Bits> { using ParentType = DebugMessageType; };
    template<> struct BitTraits<DescriptorSetLayoutCreate::Bits> { using ParentType = DescriptorSetLayoutCreate; };
    template<> struct BitTraits<Queue::Bits> { using ParentType = Queue; };
    template<> struct BitTraits<DescriptorPoolCreate::Bits> { using ParentType = DescriptorPoolCreate; };
    template<> struct BitTraits<PipelineStage::Bits> { using ParentType = PipelineStage; };
    template<> struct BitTraits<ShaderStage::Bits> { using ParentType = ShaderStage; };
    template<> struct BitTraits<Access::Bits> { using ParentType = Access; };
    template<> struct BitTraits<ImageUsage::Bits> { using ParentType = ImageUsage; };
    template<> struct BitTraits<ImageAspect::Bits> { using ParentType = ImageAspect; };
    template<> struct BitTraits<FormatFeature::Bits> { using ParentType = FormatFeature; };
    template<> struct BitTraits<FenceCreate::Bits> { using ParentType = FenceCreate; };
    template<> struct BitTraits<CommandPoolCreate::Bits> { using ParentType = CommandPoolCreate; };
    template<> struct BitTraits<CommandPoolReset::Bits> { using ParentType = CommandPoolReset; };
    template<> struct BitTraits<DebugUtils::MessengerCallbackData::Bits> { using ParentType = DebugUtils::MessengerCallbackData; };
    template<> struct BitTraits<SurfaceTransform::Bits> { using ParentType = SurfaceTransform; };
    template<> struct BitTraits<SemaphoreCreate::Bits> { using ParentType = SemaphoreCreate; };
    template<> struct BitTraits<SampleCount::Bits> { using ParentType = SampleCount; };
    template<> struct BitTraits<AttachmentCreate::Bits> { using ParentType = AttachmentCreate; };
    template<> struct BitTraits<Dependency::Bits> { using ParentType = Dependency; };
    template<> struct BitTraits<CompositeAlpha::Bits> { using ParentType = CompositeAlpha; };
    template<> struct BitTraits<PipelineLayoutCreate::Bits> { using ParentType = PipelineLayoutCreate; };
    template<> struct BitTraits<PipelineCreate::Bits> { using ParentType = PipelineCreate; };
    template<> struct BitTraits<PipelineShaderStageCreate::Bits> { using ParentType = PipelineShaderStageCreate; };
    template<> struct BitTraits<PipelineVertexInputStateCreate::Bits> { using ParentType = PipelineVertexInputStateCreate; };
    template<> struct BitTraits<PipelineInputAssemblyStateCreate::Bits> { using ParentType = PipelineInputAssemblyStateCreate; };
    template<> struct BitTraits<PipelineTessellationStateCreate::Bits> { using ParentType = PipelineTessellationStateCreate; };
    template<> struct BitTraits<PipelineViewportStateCreate::Bits> { using ParentType = PipelineViewportStateCreate; };
    template<> struct BitTraits<PipelineRasterizationStateCreate::Bits> { using ParentType = PipelineRasterizationStateCreate; };
    template<> struct BitTraits<CullMode::Bits> { using ParentType = CullMode; };
    template<> struct BitTraits<PipelineMultisampleStateCreate::Bits> { using ParentType = PipelineMultisampleStateCreate; };
    template<> struct BitTraits<PipelineDepthStencilStateCreate::Bits> { using ParentType = PipelineDepthStencilStateCreate; };
    template<> struct BitTraits<PipelineColorBlendStateCreate::Bits> { using ParentType = PipelineColorBlendStateCreate; };
    template<> struct BitTraits<PipelineDynamicStateCreate::Bits> { using ParentType = PipelineDynamicStateCreate; };
    template<> struct BitTraits<ColorComponent::Bits> { using ParentType = ColorComponent; };
    template<> struct BitTraits<DescriptorPoolReset::Bits> { using ParentType = DescriptorPoolReset; };
    template<> struct BitTraits<QueryControl::Bits> { using ParentType = QueryControl; };
    template<> struct BitTraits<QueryPipelineStatistic::Bits> { using ParentType = QueryPipelineStatistic; };
    template<> struct BitTraits<SamplerCreate::Bits> { using ParentType = SamplerCreate; };
    template<> struct BitTraits<CommandBufferReset::Bits> { using ParentType = CommandBufferReset; };
    template<> struct BitTraits<BufferViewCreate::Bits> { using ParentType = BufferViewCreate; };
    template<> struct BitTraits<MemoryMap::Bits> { using ParentType = MemoryMap; };
    template<> struct BitTraits<ImageCreate::Bits> { using ParentType = ImageCreate; };
    template<> struct BitTraits<SurfaceCreate::Bits> { using ParentType = SurfaceCreate; };
    template<> struct BitTraits<DescriptorBinding::Bits> { using ParentType = DescriptorBinding; };
    template<> struct BitTraits<DeviceCreate::Bits> { using ParentType = DeviceCreate; };
}