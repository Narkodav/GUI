#pragma once
#include "Graphics/HandleTypes/Device.h"
#include "InstanceWrapper.h"

namespace Graphics::Wrappers {
	class Device
	{
	private:
		Graphics::Device m_device;
		PhysicalDevice m_physicalDevice;
		DeviceFunctionTable m_deviceFunctions;
	public:
		Device() = default;
		Device(const Instance& instance, const PhysicalDevice& physicalDevice, const DeviceCreateInfo& createInfo)
		{
			create(instance, physicalDevice, createInfo);
		}

		~Device() {
			destroy();
		};

		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;
		Device(Device&&) = default;
		Device& operator=(Device&&) = default;

		DeviceRef getReference() const { return m_device; };
		operator DeviceRef() const { return getReference(); };

		VkDevice getHandle() const { return m_device.getHandle(); };

		void destroy() {
			if (m_device.isValid())
				m_device.destroy(m_deviceFunctions);
		}

		void create(const Instance& instance, const PhysicalDevice& physicalDevice, const DeviceCreateInfo& createInfo);

		const DeviceFunctionTable& getFunctionTable() const { return m_deviceFunctions; }

		void waitIdle();
		Queue getQueue(uint32_t familyIndex, uint32_t queueIndex);
	};
}

