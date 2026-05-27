#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../InstanceFunctionTable.h"
#include "DebugUtilsMessenger.h"

namespace Graphics
{
    class PhysicalDeviceCache;

    class PhysicalDevice;

    class AppInfo : public StructBase<VkApplicationInfo, AppInfo>
    {
        using Base = StructBase<VkApplicationInfo, AppInfo>;
    public:
        using Base::Base;

        AppInfo(const std::string& engineName,
            const std::string& appName, Version engineVersion, Version appVersion) : Base() {
            this->pApplicationName = appName.data();
            this->applicationVersion = appVersion;
            this->pEngineName = engineName.data();
            this->engineVersion = engineVersion;
        }

        AppInfo& setEngineName(const std::string& engineName) {
            this->pEngineName = engineName.c_str();
            return *this;
        }

        AppInfo& setAppName(const std::string& appName) {
            this->pApplicationName = appName.c_str();
            return *this;
        }

        AppInfo& setAppVersion(Version version) {
            this->applicationVersion = version;
            return *this;
        }

        AppInfo& setEngineVersion(Version version) {
            this->engineVersion = version;
            return *this;
        }

        std::string_view getEngineName() const { return this->pEngineName; };
        std::string_view getAppName() const { return this->pApplicationName; };
        const Version& getEngineVersion() const { return *reinterpret_cast<const Version*>(&this->engineVersion); };
        const Version& getAppVersion() const { return *reinterpret_cast<const Version*>(&this->applicationVersion); };
    };

    class LayerProperties : public StructBase<VkLayerProperties, LayerProperties>
    {
        using Base = StructBase<VkLayerProperties, LayerProperties>;
    public:
        using Base::Base;
        const std::string_view getLayerName() const { return this->layerName; };
        const Version& getImplVersion() const { return *reinterpret_cast<const Version*>(&this->implementationVersion); };
        const std::string_view getDescription() const { return this->description; };
    };

    class ExtensionProperties : public StructBase<VkExtensionProperties, ExtensionProperties>
    {
        using Base = StructBase<VkExtensionProperties, ExtensionProperties>;
    public:
        using Base::Base;
        const std::string_view getExtensionName() const { return this->extensionName; };
        const Version& getSpecVersion() const { return *reinterpret_cast<const Version*>(&this->specVersion); };
    };

    class InstanceRef : public BaseComponent<VkInstance, InstanceRef> {
        using Base = BaseComponent<VkInstance, InstanceRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Instance";

        std::vector<PhysicalDevice> getPhysicalDevices(const InstanceFunctionTable& functions) const;
        PhysicalDeviceCache cachePhysicalDevices(const InstanceFunctionTable& functions) const;
        InstanceFunctionTable getFunctionTable() const;
    };

    class InstanceCreateInfo : public StructBase<VkInstanceCreateInfo, InstanceCreateInfo>
    {
        using Base = StructBase<VkInstanceCreateInfo, InstanceCreateInfo>;
    public:
        using Base::Base;

        InstanceCreateInfo(const AppInfo& appInfo,
            std::span<const char* const> requiredLayers,
            std::span<const char* const> requiredExtensions,
            const DebugUtils::MessengerCreateInfo* debugCreateInfo = nullptr) : Base() {
            this->pApplicationInfo = AppInfo::underlyingCast(&appInfo);
            this->enabledLayerCount = requiredLayers.size();
            this->ppEnabledLayerNames = requiredLayers.data();
            this->enabledExtensionCount = requiredExtensions.size();
            this->ppEnabledExtensionNames = requiredExtensions.data();
            this->pNext = debugCreateInfo;
        }

        InstanceCreateInfo& setEnabledLayers(std::span<const char* const> requiredLayers) {
            this->enabledLayerCount = requiredLayers.size();
            this->ppEnabledLayerNames = requiredLayers.data();
            return *this;
        }

        InstanceCreateInfo& setEnabledExtensions(std::span<const char* const> requiredExtensions) {
            this->enabledExtensionCount = requiredExtensions.size();
            this->ppEnabledExtensionNames = requiredExtensions.data();
            return *this;
        }

        InstanceCreateInfo& setAppInfo(const AppInfo& appInfo) {
            this->pApplicationInfo = AppInfo::underlyingCast(&appInfo);
            return *this;
        }

        std::span<const char* const> getEnabledLayers() const {
            return std::span<const char* const>(this->ppEnabledLayerNames,
                this->enabledLayerCount);
        };

        std::span<const char* const> getEnabledExtensions() const {
            return std::span<const char* const>(this->ppEnabledExtensionNames,
                this->enabledExtensionCount);
        };

        const AppInfo& getAppInfo() const {
            return *AppInfo::underlyingCast(this->pApplicationInfo);
        };
    };

    class Instance : public VerificatorComponent<VkInstance, InstanceRef>
    {
        using Base = VerificatorComponent<VkInstance, InstanceRef>;
    public:
        using Base::Base;                

        static std::vector<LayerProperties> getAvailableValidationLayers();

        static bool validateLayers(
            std::span<const LayerProperties> availible,
            std::span<const char* const> required);

        static std::vector<const char*> enumerateRequiredExtensions(
            const std::vector<ExtensionProperties>& availibleExtensions);

        static std::vector<ExtensionProperties> getAvailableExtensions();

        static bool validateExtensions(
            std::span<const ExtensionProperties> available,
            std::span<const char* const> required);

        void create(const std::string& engineName,
            const std::string& appName, Version& engineVersion, Version& appVersion,
            std::span<const char* const> requiredValidationLayers,
            std::span<const char* const> requiredExtensions,
            DebugUtils::MessengerCreateInfo* debugCreateInfo = nullptr);

        void create(const InstanceCreateInfo& createInfo);

        void destroy(const InstanceFunctionTable& functions);
    };

}