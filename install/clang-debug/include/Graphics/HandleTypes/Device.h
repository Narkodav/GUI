#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../InstanceFunctionTable.h"
#include "../DeviceFunctionTable.h"

namespace Graphics
{
    class Queue;
    class PhysicalDevice;
    class InstanceRef;

    class QueueCreateInfo : public StructBase<VkDeviceQueueCreateInfo, QueueCreateInfo>
    {
        using Base = StructBase<VkDeviceQueueCreateInfo, QueueCreateInfo>;
    public:
        using Base::Base;

        QueueCreateInfo(uint32_t familyIndex, std::span<const float> queuePriorities) : Base()
        {
            this->pQueuePriorities = queuePriorities.data();
            this->queueCount = queuePriorities.size();
            this->queueFamilyIndex = familyIndex;
        }

        QueueCreateInfo& setQueueFamilyIndex(uint32_t index) {
            this->queueFamilyIndex = index;
            return *this;
        }

        QueueCreateInfo& setQueuePriorities(std::span<const float> queuePriorities) {
            this->pQueuePriorities = queuePriorities.data();
            this->queueCount = queuePriorities.size();
            return *this;
        }

        uint32_t getFamilyIndex() const {
            return this->queueFamilyIndex;
        }

        std::span<const float> getQueuePriorities() const {
            return std::span<const float>(this->pQueuePriorities, this->queueCount);
        }
    };

    class DeviceRef : public BaseComponent<VkDevice, DeviceRef>
    {
        using Base = BaseComponent<VkDevice, DeviceRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Device";

        DeviceFunctionTable getFunctionTable(const InstanceFunctionTable& functions) const;

        Queue getQueue(uint32_t familyIndex, uint32_t queueIndex,
            const DeviceFunctionTable& functions) const;

        void waitIdle(const DeviceFunctionTable& functions) const;
    };

    class DeviceCreateInfo : public StructBase<VkDeviceCreateInfo, DeviceCreateInfo>
    {
        using Base = StructBase<VkDeviceCreateInfo, DeviceCreateInfo>;
    public:
        using Base::Base;

        template<typename RequiredFeatures>
        DeviceCreateInfo(
            const RequiredFeatures& features,
            std::span<const char* const> extensions,
            std::span<const QueueCreateInfo> queueCreateInfos,
            std::span<const char* const> enabledLayers) : Base()
        {
            this->enabledExtensionCount = extensions.size();
            this->ppEnabledExtensionNames = extensions.data();
            this->enabledLayerCount = enabledLayers.size();
            this->ppEnabledLayerNames = enabledLayers.data();
            this->pQueueCreateInfos = QueueCreateInfo::underlyingCast(queueCreateInfos.data());
            this->queueCreateInfoCount = queueCreateInfos.size();
            this->pNext = features.getHead();
        }

        DeviceCreateInfo& setEnabledExtensions(std::span<const char* const> extensions) {
            this->enabledExtensionCount = extensions.size();
            this->ppEnabledExtensionNames = extensions.data();
            return *this;
        }

        DeviceCreateInfo& setEnabledLayers(std::span<const char* const> enabledLayers) {
            this->enabledLayerCount = enabledLayers.size();
            this->ppEnabledLayerNames = enabledLayers.data();
            return *this;
        }

        DeviceCreateInfo& setQueueCreateInfos(std::span<const QueueCreateInfo> queueCreateInfos) {
            this->pQueueCreateInfos = QueueCreateInfo::underlyingCast(queueCreateInfos.data());
            this->queueCreateInfoCount = queueCreateInfos.size();
            return *this;
        }

        template<typename RequiredFeatures>
        DeviceCreateInfo& setEnabledFeatures(const RequiredFeatures& features) {
            this->pNext = &features.getHead();
            return *this;
        }
    };

    class Device : public VerificatorComponent<VkDevice, DeviceRef>
    {
        using Base = VerificatorComponent<VkDevice, DeviceRef>;
    public:
        using Base::Base;

        void create(const InstanceFunctionTable& functions,
            const PhysicalDevice& physicalDevice,        
            const DeviceCreateInfo& createInfo);

        void destroy(const DeviceFunctionTable& functions);        
    };
}

