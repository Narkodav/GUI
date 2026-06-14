#pragma once
#include <mutex>

#include "Graphics/HandleTypes/Instance.h"
#include "Graphics/HandleTypes/Surface.h"
#include "Graphics/HandleTypes/PhysicalDeviceCache.h"
#include "DebugLogger.h"

//1. Instance & Device Management
//VkInstance - Single per application
//
//VkPhysicalDevice - Enumeration only, no creation / destruction
//
//VkDevice - Single per physical device
//
//VkQueue - Retrieved individually from device
//
//2. Surface & Display
//VkSurfaceKHR - Window system integration(one per window)
//
//VkDisplayKHR - Display objects(via extensions)
//
//VkDisplayModeKHR - Display modes
//
//VkDisplayPlaneKHR - Display planes
//
//3. SwapChain & Presentation
//VkSwapchainKHR - One per surface / window
//
//VkPresentModeKHR - Not a handle, but enumerated
//
//4. Pipeline Objects
//VkPipeline - Individual creation(bulk via extension only)
//
//VkPipelineLayout - Individual
//
//VkPipelineCache - Individual
//
//VkShaderModule - Individual
//
//5. Render Pass & Framebuffer
//VkRenderPass - Individual
//
//VkFramebuffer - Individual
//
//6. Buffer & Image Resources
//VkBuffer - Individual
//
//VkImage - Individual
//
//VkBufferView - Individual
//
//VkImageView - Individual
//
//VkSampler - Individual
//
//7. Memory
//VkDeviceMemory - Individual allocations
//
//VkAllocationCallbacks - Not a handle, but per - operation
//
//8. Query & Performance
//VkQueryPool - Individual
//
//VkPerformanceConfigurationINTEL - Extension, individual
//
//9. Deferred Operations
//VkDeferredOperationKHR - Individual(ray tracing)
//
//10. Video Coding(Extensions)
//VkVideoSessionKHR
//
//VkVideoSessionParametersKHR
//
//VkVideoPictureResourceKHR
//
//11. Acceleration Structures(Ray Tracing)
//VkAccelerationStructureKHR - Individual(though building can be batched)
//
//VkAccelerationStructureBuildGeometryInfoKHR - Info struct, not handle
//
//12. Validation & Debug
//VkDebugReportCallbackEXT - Individual(legacy)
//
//VkDebugUtilsMessengerEXT - Individual
//
//VkValidationCacheEXT - Individual
//
//13. Other Extensions
//VkIndirectCommandsLayoutNV
//
//VkMicromapEXT
//
//VkOpticalFlowSessionNV

namespace Graphics::Wrappers {

	class Instance
	{
	private:
		Graphics::Instance m_instance;
        InstanceCreateInfo m_createInfo;
        AppInfo m_appInfo;

        std::vector<ExtensionProperties> m_availibleExtensions;
        std::vector<const char*> m_requiredExtensions;
        std::vector<const char*> m_requiredLayers = { 
            "VK_LAYER_KHRONOS_validation" 
        };

        DebugUtils::MessengerCreateInfo m_debugMessengerCreateInfo;
        DebugUtils::Messenger m_debugMessenger;

		InstanceFunctionTable m_instanceFunctionTable;

        std::unique_ptr<Instance*> m_self;

        DebugLogger m_debugLogger;

        PhysicalDeviceCache m_physicalDeviceCache;

    public:
        Instance();
        Instance(const AppInfo& appInfo,
            std::span<const char*> requiredExtensions = {},
            std::span<const char*> requiredLayers = {});

		~Instance();

        Instance(const Instance&) = delete;
        Instance& operator=(const Instance&) = delete;
        Instance(Instance&& other) {
            moveImpl(std::move(other));
        }
        Instance& operator=(Instance&& other) {
            if (this == &other) return *this;
            moveImpl(std::move(other));
            return *this;
        }

        void create(const AppInfo& appInfo,
            std::span<const char*> requiredExtensions = {},
            std::span<const char*> requiredLayers = {});

        void cachePhysicalDevices();

        void destroy();

		InstanceRef getReference() const;
        operator InstanceRef() const { return getReference(); };

        VkInstance getHandle() const { return m_instance.getHandle(); };

        void dumpDebugInfo(std::ostream& stream) const;

        const InstanceFunctionTable& getFunctionTable() const { return m_instanceFunctionTable; }

        PhysicalDeviceCache::SearchResult getFittingDevice(SurfaceRef surface, const DeviceRequirements& requirements) const {
            auto result = m_physicalDeviceCache.getFittingDevice(m_instanceFunctionTable, surface, requirements);
            if (!result.isSuitable()) {
                throw std::runtime_error("No fitting device found");
            }
            return result;
        }

    private:
        void moveImpl(Instance&& other) {
            m_instance = std::exchange(other.m_instance, Graphics::Instance());
            m_createInfo = std::exchange(other.m_createInfo, InstanceCreateInfo());
            m_appInfo = std::exchange(other.m_appInfo, AppInfo());

            m_availibleExtensions = std::exchange(other.m_availibleExtensions, std::vector<ExtensionProperties>());
            m_requiredExtensions = std::exchange(other.m_requiredExtensions, std::vector<const char*>());
            m_requiredLayers = std::exchange(other.m_requiredLayers, std::vector<const char*>());

            m_debugMessengerCreateInfo = std::exchange(other.m_debugMessengerCreateInfo, DebugUtils::MessengerCreateInfo());
            m_debugMessenger = std::exchange(other.m_debugMessenger, DebugUtils::Messenger());

            m_instanceFunctionTable = std::exchange(other.m_instanceFunctionTable, InstanceFunctionTable());

            m_self = std::exchange(other.m_self, nullptr);
            updateSelf();

            m_debugLogger = std::exchange(other.m_debugLogger, DebugLogger());
        }

        static GRAPHICS_API_ATTR Bool32 GRAPHICS_API_CALL static_debugCallback(
            Flags::DebugMessageSeverity::Bits messageSeverity,
            Flags::DebugMessageType messageType,
            const DebugUtils::CallbackData* pCallbackData,
            void* pUserData);

        Bool32 debugCallback(
            Flags::DebugMessageSeverity::Bits messageSeverity,
            Flags::DebugMessageType messageType,
            const DebugUtils::CallbackData* pCallbackData);

        void updateSelf();
        void createSelf();

        void createImpl(const AppInfo& appInfo,
            std::span<const char*> requiredExtensions,
            std::span<const char*> requiredLayers);
	};
}
