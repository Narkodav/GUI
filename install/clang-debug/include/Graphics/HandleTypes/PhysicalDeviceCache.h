#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../InstanceFunctionTable.h"
#include "Instance.h"
#include "PhysicalDevice.h"
#include "Queue.h"
#include "../TaskTables/QueuePropertyEnum.h"
#include "../TaskTables/PropertyEnum.h"
#include "../TaskTables/FeatureEnum.h"

namespace Graphics
{
	using RequiredFeatures = std::unordered_map<DeviceFeature, std::any>;
	using RequiredProperties = std::unordered_map<DeviceProperty, std::any>;
	using RequiredExtensions = std::vector<const char*>;

	struct RequiredQueueProperties
	{
		std::unordered_map<QueueProperty, std::any> queueProperties;
		bool shouldSupportPresent = false;
	};

	struct DeviceRequirements
	{
		RequiredFeatures features;
		RequiredProperties properties;
		RequiredExtensions extensions;

		std::vector<RequiredQueueProperties> queueProperties;
	};

	//this a fully custom structure that simplifies device selection
	class PhysicalDeviceCache
	{
		friend class InstanceRef;

	public:
		struct Data {
			std::vector<CompleteQueueFamilyPropertyChain> m_families;
			PhysicalDevice::CompletePropertyChain m_properties;
			PhysicalDevice::CompleteFeatureChain m_features;
			std::vector<ExtensionProperties> m_availableExtensions;
		};

		struct SearchResult
		{
			PhysicalDevice device;
			PhysicalDevice::CompleteFeatureChain enabledFeatures;
			std::vector<std::vector<uint32_t>> queueFamilyIndices;

			inline bool isSuitable() { return device != nullptr; };

			static SearchResult unsuitable() { return SearchResult(); };
			static SearchResult suitable(PhysicalDevice device, PhysicalDevice::CompleteFeatureChain&& enabledFeatures,
				std::vector<std::vector<uint32_t>>&& queueFamilyIndices)
			{
				return SearchResult{ device, std::move(enabledFeatures), std::move(queueFamilyIndices) };
			};
		};

	private:

		std::vector<std::pair<PhysicalDevice, Data>> m_data;

		PhysicalDeviceCache(InstanceRef instance, const InstanceFunctionTable& functions);

	public:
		PhysicalDeviceCache() = default;
		PhysicalDeviceCache(PhysicalDeviceCache&&) = default;
		PhysicalDeviceCache& operator=(PhysicalDeviceCache&&) = default;
		PhysicalDeviceCache(const PhysicalDeviceCache&) = delete;
		PhysicalDeviceCache& operator=(const PhysicalDeviceCache&) = delete;

		SearchResult getFittingDevice(const InstanceFunctionTable& functions,
			SurfaceRef surface, const DeviceRequirements& requirements) const;

		SearchResult getFittingDevice(
			const DeviceRequirements& requirements) const;

		void checkDeviceSuitability(SearchResult& result,
			const InstanceFunctionTable& functions, SurfaceRef surface, 
			const std::pair<PhysicalDevice, PhysicalDeviceCache::Data>& data,
			const DeviceRequirements& requirements) const;

		void checkDeviceSuitability(
			SearchResult& result, const DeviceRequirements& requirements, 
			const std::pair<PhysicalDevice, PhysicalDeviceCache::Data>& data) const;

		bool checkQueueFamilySuitability(
			const InstanceFunctionTable& functions, SurfaceRef surface,
			const std::unordered_map<QueueProperty, std::any>& props,
			const std::pair<PhysicalDevice, PhysicalDeviceCache::Data>& data,
			size_t familyIndex) const;

		bool checkQueueFamilySuitability(
			const std::unordered_map<QueueProperty, std::any>& props,
			const std::pair<PhysicalDevice, PhysicalDeviceCache::Data>& data,
			size_t familyIndex) const;

	private:
		void addDevice(const InstanceFunctionTable& functions, const PhysicalDevice& device);
	};
}

