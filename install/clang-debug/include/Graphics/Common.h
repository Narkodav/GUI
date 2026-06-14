#pragma once
//#pragma warning(error: 4715)

//vulkan
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
//#include <vma/vk_mem_alloc.h>

//glm
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RIGHT_HANDED
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <optional>
#include <functional>
#include <any>
#include <ranges>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <atomic>
#include <set>
#include <fstream>
#include <array>
#include <stack>
#include <chrono>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <span>
#include <source_location>
#include <type_traits>

#include "StructTraits.h"

#include "Utilities/TemplateUnion.h"

#define GRAPHICS_API_ATTR VKAPI_ATTR
#define GRAPHICS_API_CALL VKAPI_CALL
#define GRAPHICS_API_PTR  VKAPI_PTR
#define GRAPHICS_VERIFY(condition, message) { \
    auto __LOCATION_VARIABLE__ = std::source_location::current(); \
    Graphics::verify(condition, message, \
        __LOCATION_VARIABLE__.function_name(), \
        __LOCATION_VARIABLE__.file_name(), \
        __LOCATION_VARIABLE__.line(), \
        __LOCATION_VARIABLE__.column()); \
}

#define GRAPHICS_VERIFY_RESULT(result, message) { \
if constexpr (std::convertible_to<decltype(result), Graphics::Result>) { \
    GRAPHICS_VERIFY(static_cast<Graphics::Result>(result) == Graphics::Result::Success, \
        std::string(message) + ": " + ResultManager::getResultMessage(result).data()); \
} \
else if constexpr (std::convertible_to<decltype(result), vk::Result>) { \
    \
    GRAPHICS_VERIFY(static_cast<vk::Result>(result) == vk::Result::eSuccess, \
        std::string(message) + ": " + ResultManager::getResultMessage(result).data()); \
} \
else { \
    GRAPHICS_VERIFY(static_cast<uint32_t>(result) == VK_SUCCESS, \
        std::string(message) + ": " + ResultManager::getResultMessage(result).data()); \
} \
} \

#if defined(GRAPHICS_NO_VERIFY) && defined(GRAPHICS_ALWAYS_VERIFY)
#error "GRAPHICS_NO_VERIFY and GRAPHICS_ALWAYS_VERIFY cannot be defined at the same time"
#endif

//asserts trivially copyable and layout identity for structs
#define GRAPHICS_ASSERT_STRUCT_PROPERTIES(GraphicsStruct, VulkanStruct) \
static_assert(sizeof(GraphicsStruct) == sizeof(VulkanStruct)); \
static_assert(alignof(GraphicsStruct) == alignof(VulkanStruct)); \
static_assert(std::is_trivially_copyable_v<GraphicsStruct>); \
static_assert(std::is_standard_layout_v<GraphicsStruct>) \

//asserts trivially copyable and trivially default constructible and layout identity for unions
#define GRAPHICS_ASSERT_UNION_PROPERTIES(GraphicsUnion, VulkanUnion) \
static_assert(sizeof(GraphicsUnion) == sizeof(VulkanUnion)); \
static_assert(alignof(GraphicsUnion) == alignof(VulkanUnion)); \
static_assert(std::is_trivially_copyable_v<GraphicsUnion>); \
static_assert(std::is_standard_layout_v<GraphicsUnion>) \


//assert object binary layout
#define GRAPHICS_ASSERT_HANDLE_PROPERTIES(GraphicsHandle) \
static_assert(sizeof(GraphicsHandle) == sizeof(uintptr_t)); \
static_assert(alignof(GraphicsHandle) == alignof(uintptr_t)); \
static_assert(std::is_standard_layout_v<GraphicsHandle>) \

//assert object reference binary layout
#define GRAPHICS_ASSERT_HANDLE_REFERENCE_PROPERTIES(GraphicsHandleRef) \
GRAPHICS_ASSERT_HANDLE_PROPERTIES(GraphicsHandleRef); \
static_assert(std::is_trivially_copyable_v<GraphicsHandleRef>); \
static_assert(std::is_trivially_default_constructible_v<GraphicsHandleRef>) \

namespace Graphics {

    using DeviceSize = VkDeviceSize;

    static inline void verify(bool condition, std::string_view message, const char* function, const char* file, int line, int column) {
#if !defined(NODEBUG) && !defined(GRAPHICS_NO_VERIFY) || defined(GRAPHICS_ALWAYS_VERIFY)
        if (!condition) {
            std::stringstream formatted;
            formatted << "[VERIFY FAILED] " << message <<
                "\n  File: " << file <<
                "\n  Function: " << function <<
                "\n  Line: " << line <<
                "\n  Column: " << column << std::endl;
            std::cerr << formatted.str();
            throw std::runtime_error(formatted.str());
        }
#else
        (void)condition; (void)message; (void)file; (void)line; (void)func;
#endif
    }

    using SampleMask = VkSampleMask;

    template<typename T>
    concept StructHasNext = requires(T t) { t.pNext; };

    template<typename T>
    concept StructHasSType = requires(T t) { t.sType; };

    template<typename T>
    concept CStruct =
        std::is_standard_layout_v<T> &&
        std::is_trivially_default_constructible_v<T> &&
        std::is_trivially_copyable_v<T> &&
        std::is_aggregate_v<T>;

    template<CStruct T, typename Derived>
    class StructBase : public T {
    public:
        using T::T;

        constexpr StructBase() : T{}
        {
            if constexpr (StructHasSType<T>) {
                this->sType = static_cast<std::remove_const_t<decltype(this->sType)>>(
                    StructToEnumTraits_v<T>
                    );
            }
        }

        constexpr StructBase(const StructBase& other) = default;
        constexpr StructBase(StructBase&& other) = default;

        constexpr StructBase& operator=(const StructBase& other)  = default;
        constexpr StructBase& operator=(StructBase&& other)  = default;

        constexpr StructBase(const T& other) : T(other) {}
        constexpr StructBase(T&& other) : T(std::move(other)) {}

        constexpr StructBase& operator=(const T& other) { static_cast<T&>(*this) = other; return *this; };
        constexpr StructBase& operator=(T&& other) { static_cast<T&>(*this) = std::move(other); return *this; };

        constexpr operator T& () { return static_cast<T&>(*this); }
        constexpr operator const T& () const { return static_cast<const T&>(*this); }
        constexpr const T* getUnderlyingPointer() const { return static_cast<const T*>(this); };
        constexpr T* getUnderlyingPointer() { return static_cast<T*>(this); };

        constexpr const T& getStruct() const { return static_cast<const T&>(*this); };
        constexpr T& getStruct() { return static_cast<T&>(*this); };

        constexpr static const T* underlyingCast(const Derived* ptr) { return static_cast<const T*>(ptr); };
        constexpr static T* underlyingCast(Derived* ptr) { return static_cast<T*>(ptr); };

        constexpr static const T& underlyingCast(const Derived& ref) { return static_cast<const T&>(ref); };
        constexpr static T& underlyingCast(Derived& ref) { return static_cast<T&>(ref); };

        constexpr static const Derived* underlyingCast(const T* ptr) { return static_cast<const Derived*>(ptr); };
        constexpr static Derived* underlyingCast(T* ptr) { return static_cast<Derived*>(ptr); };

        constexpr static const Derived& underlyingCast(const T& ref) { return *static_cast<const Derived*>(&ref); };
        constexpr static Derived& underlyingCast(T& ref) { return *static_cast<Derived*>(&ref); };

        constexpr const void* getNext() const requires StructHasNext<T> { return this->pNext; };
        constexpr Derived& setNext(void* pNext) requires StructHasNext<T> {
            this->pNext = pNext;
            return *static_cast<Derived*>(this);
        };

        constexpr StructureType getType() const requires StructHasSType<T> { return convertEnum(this->sType); }
    };

    template<typename T, typename... Ts>
    struct TupleHasTypeTrait {};

    template<typename T, typename First, typename... Ts>
    struct TupleHasTypeTrait<T, First, Ts...> {
        static constexpr bool value = std::is_same_v<T, First> || TupleHasTypeTrait<T, Ts...>::value;
    };

    template<typename T, typename Last>
    struct TupleHasTypeTrait<T, Last> {
        static constexpr bool value = std::is_same_v<T, Last> || false;
    };

    template<typename T, typename... Ts>
    static constexpr bool TupleHasTypeTrait_v = TupleHasTypeTrait<T, Ts...>::value;

    template<typename T, typename Derived>
    class UnionBase {
    public:
        constexpr UnionBase() = default;
        constexpr ~UnionBase() = default;
        constexpr UnionBase(const UnionBase&) = default;
        constexpr UnionBase(UnionBase&&) = default;

        constexpr UnionBase& operator=(const UnionBase&) = default;
        constexpr UnionBase& operator=(UnionBase&&) = default;

        constexpr operator T& () { return getUnion(); }
        constexpr operator const T& () const { return getUnion(); }
        constexpr const T* getUnderlyingPointer() const { return reinterpret_cast<const T*>(this); };
        constexpr T* getUnderlyingPointer() { return reinterpret_cast<T*>(this); };

        static const T* underlyingCast(const Derived* ptr) { return reinterpret_cast<const T*>(ptr); };
        static T* underlyingCast(Derived* ptr) { return reinterpret_cast<T*>(ptr); };

        const T& getUnion() const { return *reinterpret_cast<const T*>(this); };
        T& getUnion() { return *reinterpret_cast<T*>(this); };

        static const Derived* underlyingCast(const T* ptr) { return reinterpret_cast<const Derived*>(ptr); };
        static Derived* underlyingCast(T* ptr) { return reinterpret_cast<Derived*>(ptr); };
    };

    template<typename T, typename Derived>
    class BaseComponent {
    protected:
        T m_handle;

        template<typename U>
        constexpr void setHandle(U&& newHandle) { m_handle = std::forward<U>(newHandle); };
        constexpr T& getHandleInternal() { return m_handle; };
        constexpr void reset() { m_handle = nullptr; };

        constexpr T* getUnderlyingPointer() { return &m_handle; };
        constexpr operator T& () { return this->getHandleInternal(); };

    public:
        using VulkanType = T;

        constexpr BaseComponent() noexcept = default;
        constexpr ~BaseComponent() noexcept = default;

        constexpr BaseComponent(BaseComponent&&) noexcept = default;
        constexpr BaseComponent& operator=(BaseComponent&&) noexcept = default;

        constexpr BaseComponent(const BaseComponent&) noexcept = default;
        constexpr BaseComponent& operator=(const BaseComponent&) noexcept = default;

        constexpr BaseComponent(T&& handle) noexcept : m_handle(std::move(handle)) {}

        constexpr BaseComponent& operator=(T&& handle) noexcept {
            m_handle = std::move(handle);
            return *this;
        };

        constexpr BaseComponent(const T& handle) noexcept : m_handle(handle) {}

        constexpr BaseComponent& operator=(const T& handle) noexcept {
            m_handle = handle;
            return *this;
        };

        constexpr const T& getHandle() const { return m_handle; };
        constexpr uintptr_t getNumerical() const { return reinterpret_cast<uintptr_t>(m_handle); };

        constexpr bool operator==(const BaseComponent& other) const { return this->getHandle() == other.getHandle(); };
        constexpr bool operator!=(const BaseComponent& other) const { return this->getHandle() != other.getHandle(); };
        constexpr bool operator==(const T& handle) const { return this->getHandle() == handle; };
        constexpr bool operator!=(const T& handle) const { return this->getHandle() != handle; };

        constexpr operator const T& () const { return this->getHandle(); };
        constexpr const T* getUnderlyingPointer() const { return &m_handle; };

        constexpr static const T* underlyingCast(const BaseComponent* ptr) { return reinterpret_cast<const T*>(ptr); };
        constexpr static T* underlyingCast(BaseComponent* ptr) { return reinterpret_cast<T*>(ptr); };
        constexpr static const Derived* underlyingCast(const T* ptr) { return reinterpret_cast<const Derived*>(ptr); };
        constexpr static Derived* underlyingCast(T* ptr) { return reinterpret_cast<Derived*>(ptr); };

        constexpr bool isSet() const { return m_handle != nullptr; };
    };

    template <typename T>
    concept RefType = requires {
        T::s_typeName;
        requires std::is_same_v<decltype(T::s_typeName), const std::string> ||
        std::is_same_v<decltype(T::s_typeName), const char*>;
    };

    template<typename T, RefType Ref>
    class VerificatorComponent : public Ref {
    public:
        VerificatorComponent() { this->setHandle(nullptr); };

        VerificatorComponent(VerificatorComponent&& other) {
            this->setHandle(other.getHandle());
            other.reset();
        }

        VerificatorComponent& operator=(VerificatorComponent&& other) {
            if (this == &other)
                return *this;
            GRAPHICS_VERIFY(!isValid(), std::string("Trying to move to valid object: ") + Ref::s_typeName);
            this->setHandle(other.getHandle());
            other.reset();
            return *this;
        }

        ~VerificatorComponent() {
            GRAPHICS_VERIFY(!isValid(), std::string("Valid object was not destroyed: ") + Ref::s_typeName);
        };

        VerificatorComponent(T&& other) {
            this->setHandle(std::move(other));
        };

        VerificatorComponent& operator=(T&& other) {
            if (this == reinterpret_cast<VerificatorComponent*>(&other))
                return *this;
            GRAPHICS_VERIFY(!isValid(), std::string("Trying to move to valid object: ") + Ref::s_typeName);
            this->setHandle(std::move(other));
            return *this;
        }

        VerificatorComponent(const VerificatorComponent&) = delete;
        VerificatorComponent& operator=(const VerificatorComponent&) = delete;
        VerificatorComponent(const T&) = delete;
        VerificatorComponent& operator=(const T&) = delete;

        bool isValid() const { return this->isSet(); };

        Ref getReference() const { return Ref(this->getHandle()); };
    };

    using Bool32 = VkBool32;

    class ResultManager {
    public:
        static inline const std::unordered_map<Result, const std::string> s_resultMessages = {
            {Result::ErrorUnknown, "Unknown error"},
            {Result::Success, "Success"},
            {Result::NotReady, "A fence or query has not yet completed"},
            {Result::Timeout, "A wait operation has not completed in the specified time"},
            {Result::EventSet, "An event is signaled"},
            {Result::EventReset, "An event is unsignaled"},
            {Result::Incomplete, "A return array was too small for the result"},

            {Result::ErrorOutOfHostMemory, "Out of host memory"},
            {Result::ErrorOutOfDeviceMemory, "Out of device memory"},
            {Result::ErrorInitializationFailed, "Initialization failed"},
            {Result::ErrorDeviceLost, "The logical or physical device has been lost"},
            {Result::ErrorMemoryMapFailed, "Mapping of a memory object has failed"},
            {Result::ErrorLayerNotPresent, "A requested layer is not present or could not be loaded"},
            {Result::ErrorExtensionNotPresent, "A requested extension is not supported"},
            {Result::ErrorFeatureNotPresent, "A requested feature is not supported"},
            {Result::ErrorIncompatibleDriver, "The requested version of Vulkan is not supported by the driver"},
            {Result::ErrorTooManyObjects, "Too many objects of the type have already been created"},
            {Result::ErrorFormatNotSupported, "A requested format is not supported on this device"},
            {Result::ErrorFragmentedPool, "A pool allocation has failed due to fragmentation"},
            {Result::ErrorUnknown, "An unknown error has occurred"},

            {Result::ErrorOutOfPoolMemory, "Out of pool memory"},
            {Result::ErrorInvalidExternalHandle, "An external handle is invalid"},
            {Result::ErrorFragmentation, "A fence or query has not yet completed"},
            {Result::ErrorInvalidOpaqueCaptureAddress, "Invalid opaque capture address"},
            {Result::PipelineCompileRequired, "Pipeline compile required"},
            {Result::ErrorNotPermitted, "Operation not permitted"},

            {Result::ErrorSurfaceLostKHR, "Surface lost"},
            {Result::ErrorNativeWindowInUseKHR, "Native window in use"},
            {Result::ErrorOutOfDateKHR, "Swapchain out of date"},
            {Result::ErrorIncompatibleDisplayKHR, "Incompatible display"},
            {Result::ErrorValidationFailedEXT, "Validation failed external"},
            {Result::ErrorInvalidShaderNV, "Invalid shader NVIDIA"},

            {Result::ErrorImageUsageNotSupportedKHR, "Image usage not supported"},
            {Result::ErrorVideoPictureLayoutNotSupportedKHR, "Video picture layout not supported"},
            {Result::ErrorVideoProfileOperationNotSupportedKHR, "Video profile operation not supported"},
            {Result::ErrorVideoProfileFormatNotSupportedKHR, "Video profile format not supported"},
            {Result::ErrorVideoProfileCodecNotSupportedKHR, "Video profile codec not supported"},
            {Result::ErrorVideoStdVersionNotSupportedKHR, "Video standard version not supported"},

            {Result::ErrorInvalidDrmFormatModifierPlaneLayoutEXT, "Invalid DRM format modifier plane layout"},
            {Result::ErrorFullScreenExclusiveModeLostEXT, "Full screen exclusive mode lost"},

            {Result::ThreadIdleKHR, "Thread idle"},
            {Result::ThreadDoneKHR, "Thread done"},
            {Result::OperationDeferredKHR, "Operation deferred"},
            {Result::OperationNotDeferredKHR, "Operation not deferred"},

            {Result::ErrorInvalidVideoStdParametersKHR, "Invalid video standard parameters"},
            {Result::ErrorCompressionExhaustedEXT, "Compression exhausted"},

            {Result::IncompatibleShaderBinaryEXT, "Incompatible shader binary"},
            {Result::PipelineBinaryMissingKHR, "Pipeline binary missing"},
            {Result::ErrorNotEnoughSpaceKHR, "Not enough space"},

            // Aliases - for completeness
            {Result::ErrorOutOfPoolMemoryKHR, "Out of pool memory (KHR alias)"},
            {Result::ErrorInvalidExternalHandleKHR, "Invalid external handle (KHR alias)"},
            {Result::ErrorFragmentationEXT, "Fragmentation error (EXT alias)"},
            {Result::ErrorNotPermittedEXT, "Operation not permitted (EXT alias)"},
            {Result::ErrorNotPermittedKHR, "Operation not permitted (KHR alias)"},
            {Result::ErrorInvalidDeviceAddressEXT, "Invalid device address (EXT alias)"},
            {Result::ErrorInvalidOpaqueCaptureAddressKHR, "Invalid opaque capture address (KHR alias)"},
            {Result::PipelineCompileRequiredEXT, "Pipeline compile required (EXT alias)"},
            {Result::ErrorPipelineCompileRequiredEXT, "Pipeline compile required error (EXT alias)"},
            {Result::ErrorIncompatibleShaderBinaryEXT, "Incompatible shader binary (EXT alias)"}
        };

        static std::string_view getResultMessage(Result result) {
            if (s_resultMessages.find(result) != s_resultMessages.end())
                return s_resultMessages.at(result);
            else
                return s_resultMessages.at(Result::ErrorUnknown);
        }

        static std::string_view getResultMessage(VkResult result) {
            return getResultMessage(static_cast<Result>(result));
        }

        static std::string_view getResultMessage(vk::Result result) {
            return getResultMessage(static_cast<Result>(result));
        }
    };

    class ObjectTypeManager {
    public:
        static inline const std::unordered_map<ObjectType, std::string> s_objectTypeNames = {
            {ObjectType::Unknown, "Unknown"},
            {ObjectType::Instance, "Instance"},
            {ObjectType::PhysicalDevice, "PhysicalDevice"},
            {ObjectType::Device, "Device"},
            {ObjectType::Queue, "Queue"},
            {ObjectType::Semaphore, "Semaphore"},
            {ObjectType::CommandBuffer, "CommandBuffer"},
            {ObjectType::Fence, "Fence"},
            {ObjectType::DeviceMemory, "DeviceMemory"},
            {ObjectType::Buffer, "Buffer"},
            {ObjectType::Image, "Image"},
            {ObjectType::Event, "Event"},
            {ObjectType::QueryPool, "QueryPool"},
            {ObjectType::BufferView, "BufferView"},
            {ObjectType::ImageView, "ImageView"},
            {ObjectType::ShaderModule, "ShaderModule"},
            {ObjectType::PipelineCache, "PipelineCache"},
            {ObjectType::PipelineLayout, "PipelineLayout"},
            {ObjectType::RenderPass, "RenderPass"},
            {ObjectType::Pipeline, "Pipeline"},
            {ObjectType::DescriptorSetLayout, "DescriptorSetLayout"},
            {ObjectType::Sampler, "Sampler"},
            {ObjectType::DescriptorPool, "DescriptorPool"},
            {ObjectType::DescriptorSet, "DescriptorSet"},
            {ObjectType::Framebuffer, "Framebuffer"},
            {ObjectType::CommandPool, "CommandPool"},
            {ObjectType::SurfaceKHR, "SurfaceKHR"},
            {ObjectType::SwapchainKHR, "SwapchainKHR"},
            {ObjectType::DisplayKHR, "DisplayKHR"},
            {ObjectType::DisplayModeKHR, "DisplayModeKHR"},
            {ObjectType::DebugReportCallbackEXT, "DebugReportCallbackEXT"},
            {ObjectType::DebugUtilsMessengerEXT, "DebugUtilsMessengerEXT"},
            {ObjectType::ValidationCacheEXT, "ValidationCacheEXT"},
            {ObjectType::AccelerationStructureKHR, "AccelerationStructureKHR"},
            {ObjectType::AccelerationStructureNV, "AccelerationStructureNV"},
            {ObjectType::PerformanceConfigurationINTEL, "PerformanceConfigurationINTEL"},
            {ObjectType::DeferredOperationKHR, "DeferredOperationKHR"},
            {ObjectType::IndirectCommandsLayoutNV, "IndirectCommandsLayoutNV"},
            {ObjectType::PrivateDataSlot, "PrivateDataSlot"},
            {ObjectType::VideoSessionKHR, "VideoSessionKHR"},
            {ObjectType::VideoSessionParametersKHR, "VideoSessionParametersKHR"},
            {ObjectType::CuModuleNVX, "CuModuleNVX"},
            {ObjectType::CuFunctionNVX, "CuFunctionNVX"},
            {ObjectType::OpticalFlowSessionNV, "OpticalFlowSessionNV"},
            {ObjectType::MicromapEXT, "MicromapEXT"},
            {ObjectType::ShaderEXT, "ShaderEXT"}
        };

        static std::string_view getObjectTypeName(ObjectType objectType) {
            if (s_objectTypeNames.find(objectType) != s_objectTypeNames.end())
                return s_objectTypeNames.at(objectType);
            else
                return s_objectTypeNames.at(ObjectType::Unknown);
        }

        static std::string_view getObjectTypeName(VkObjectType objectType) {
            return getObjectTypeName(static_cast<ObjectType>(objectType));
        }

        static std::string_view getObjectTypeName(vk::ObjectType objectType) {
            return getObjectTypeName(static_cast<ObjectType>(objectType));
        }
    };

    template<typename T, typename... Types>
    struct IndexFromVariadicPack
    {
    private:
        template<typename U, typename... Ts>
        struct IndexFromVariadicPackImpl;

        template<typename U, typename First, typename... Rest>
        struct IndexFromVariadicPackImpl<U, First, Rest...> {
            static inline constexpr size_t index = std::is_same_v<U, First> ? 0 : 1 + IndexFromVariadicPackImpl<U, Rest...>::index;
        };

        template<typename U>
        struct IndexFromVariadicPackImpl<U> {
            static inline constexpr size_t index = 0;
        };
    public:
        static inline constexpr size_t index = IndexFromVariadicPackImpl<T, Types...>::index;
        static_assert(index < sizeof...(Types), "Type index not found in variadic pack");
    };

    template<typename T, typename... Types>
    static inline const auto IndexFromVariadicPack_v = IndexFromVariadicPack<T, Types...>::index;

    template<int32_t index, typename... Types>
    struct TypeFromVariadicPackIndex;

    template<int32_t index, typename First, typename... Rest>
    struct TypeFromVariadicPackIndex<index, First, Rest...> {
        using Type = std::conditional_t<(index == 0), First, typename TypeFromVariadicPackIndex<index - 1, Rest...>::Type>;
    };

    template<int32_t index>
    struct TypeFromVariadicPackIndex<index> {
        using Type = void;
        static_assert(index < 0, "Index out of bounds");
    };

    template<int32_t index, typename... Types>
    using TypeFromVariadicPackIndex_t = typename TypeFromVariadicPackIndex<index, Types...>::Type;

    template<StructureType... structs>
    struct StructChain {
    private:
        std::tuple<EnumToStructTraits_ct<structs>...> m_structs;

        void link() {
            if constexpr (sizeof...(structs) > 1) {
                [this] <size_t... I>(std::index_sequence<I...>) {
                    ((std::get<I>(m_structs).pNext = &std::get<I + 1>(m_structs)), ...);
                    ((std::get<I>(m_structs).sType = static_cast<VkStructureType>(
                        StructToEnumTraits_v<TypeFromVariadicPackIndex_t<I, EnumToStructTraits_ct<structs>...>>
                        )), ...);
                }(std::make_index_sequence<sizeof...(structs) - 1>{});
            }
            std::get<sizeof...(structs) - 1>(m_structs).sType = static_cast<VkStructureType>(
                StructToEnumTraits_v<TypeFromVariadicPackIndex_t<sizeof...(structs) - 1, EnumToStructTraits_ct<structs>...>>);
        }

    public:
        using HeadType = std::tuple_element_t<0, std::tuple<EnumToStructTraits_ct<structs>...>>;

        StructChain() : m_structs({}) {
            link();
        }

        ~StructChain() = default;

        StructChain(StructChain&& other) {
            m_structs = std::move(other.m_structs);
            link();
        }

        StructChain& operator=(StructChain&& other) {
            if (this == &other)
                return *this;

            m_structs = std::move(other.m_structs);
            link();
            return *this;
        }

        StructChain(const StructChain& other) {
            m_structs = other.m_structs;
            link();
        }

        StructChain& operator=(const StructChain& other) {
            if (this == &other)
                return *this;

            m_structs = other.m_structs;
            link();
            return *this;
        }

        template<size_t index>
        inline auto& get() {
            return std::get<index>(m_structs);
        }

        template<StructureType structure>
        inline auto& get() {
            return std::get<
                IndexFromVariadicPack_v<EnumToStructTraits_ct<structure>, EnumToStructTraits_ct<structs>...>
            >(m_structs);
        }

        template<size_t index>
        inline const auto& get() const {
            return std::get<index>(m_structs);
        }

        template<StructureType structure>
        inline const auto& get() const {
            return std::get<
                IndexFromVariadicPack_v<EnumToStructTraits_ct<structure>, EnumToStructTraits_ct<structs>...>
            >(m_structs);
        }

        inline HeadType& getHead() {
            return std::get<0>(m_structs);
        }

        inline const HeadType& getHead() const {
            return std::get<0>(m_structs);
        }
    };

    template<typename T>
    concept HandleType = requires(T container) {
        typename T::VulkanType;
        requires T::hasIdenticalMemoryLayoutToInteger == true;
    };

    template<typename T>
    concept BasicContainer = requires(T container) {
        typename T::value_type;
        typename T::size_type;
    };

    template<typename T>
    concept ContiguousContainer = BasicContainer<T> && requires(T container) {
        { std::as_const(container).data() } -> std::convertible_to<const typename T::value_type*>;
        { std::as_const(container).size() } -> std::convertible_to<typename T::size_type>;
    };

    template<HandleType T>
    static uint64_t convertHandle(const T& handle) {
        return reinterpret_cast<uint64_t>(handle.getHandle());
    }

} // namespace Graphics