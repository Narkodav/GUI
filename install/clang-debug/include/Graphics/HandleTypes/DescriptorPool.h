#pragma once
#include "Graphics/Common.h"
#include "Graphics/Structs.h"
#include "Graphics/DeviceFunctionTable.h"
#include "Graphics/HandleTypes/Device.h"
#include "Graphics/HandleTypes/Sampler.h"
#include "Graphics/HandleTypes/DescriptorSet.h"

namespace Graphics
{
	class DescriptorPoolRef;

	class DescriptorSetAllocateInfo :
		public StructBase<VkDescriptorSetAllocateInfo, DescriptorSetAllocateInfo>
	{
		using Base = StructBase<VkDescriptorSetAllocateInfo, DescriptorSetAllocateInfo>;
	public:
		using Base::Base;

		DescriptorSetAllocateInfo(DescriptorPoolRef descriptorPool, std::span<const DescriptorSetLayoutRef> layouts);
		DescriptorSetAllocateInfo(DescriptorPoolRef descriptorPool, std::span<const DescriptorSetLayout> layouts);

		template <ContiguousContainer Container>
		DescriptorSetAllocateInfo& setDescriptorSetLayouts(const Container& layouts) requires
			std::same_as<typename Container::value_type, DescriptorSetLayoutRef> ||
			std::same_as<typename Container::value_type, DescriptorSetLayout> {
			this->descriptorSetCount = layouts.size();
			this->pSetLayouts = DescriptorSetLayout::underlyingCast(layouts.data());
			return *this;
		}

		DescriptorSetAllocateInfo& setDescriptorSetLayouts(const DescriptorSetLayoutRef* layouts, size_t count) {
			this->descriptorSetCount = count;
			this->pSetLayouts = DescriptorSetLayout::underlyingCast(layouts);
			return *this;
		}

		DescriptorSetAllocateInfo& setDescriptorSetLayouts(DescriptorSetLayoutRef layout) {
			this->descriptorSetCount = 1;
			this->pSetLayouts = DescriptorSetLayout::underlyingCast(&layout);
			return *this;
		}

		DescriptorSetAllocateInfo& setDescriptorPool(DescriptorPoolRef descriptorPool);

		std::span<const DescriptorSetLayoutRef> getLayouts() const {
			return std::span<const DescriptorSetLayoutRef>(
				DescriptorSetLayoutRef::underlyingCast(this->pSetLayouts), this->descriptorSetCount);
		}

		size_t getDescriptorSetCount() const {
			return this->descriptorSetCount;
		}
	};

	class DescriptorPoolRef : public BaseComponent<VkDescriptorPool, DescriptorPoolRef>
	{
		using Base = BaseComponent<VkDescriptorPool, DescriptorPoolRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "DescriptorPool";

		void reset(const DeviceFunctionTable& functions, DeviceRef device,
			Flags::DescriptorPoolReset flags = Flags::DescriptorPoolReset::Bits::None);

		static std::vector<DescriptorSet> allocateSets(const DeviceFunctionTable& functions,
			DeviceRef device, const DescriptorSetAllocateInfo& allocateInfo);

		template<size_t count>
		std::array<DescriptorSet, count> allocateSets(const DeviceFunctionTable& functions,
			DeviceRef device, const DescriptorSetAllocateInfo& allocateInfo)
		{
			GRAPHICS_VERIFY(allocateInfo.getDescriptorSetCount() == count, "descriptor count must match count in allocateInfo");
			std::array<DescriptorSet, count> sets;
			auto result = functions.execute<DeviceFunction::AllocateDescriptorSets>(
				device.getHandle(), allocateInfo.getUnderlyingPointer(),
				DescriptorSet::underlyingCast(sets.data()));
			GRAPHICS_VERIFY_RESULT(result, "Failed to allocate sets from descriptor pool");
			return sets;
		}

		void freeSets(const DeviceFunctionTable& functions, DeviceRef device, std::span<const DescriptorSet> sets);
		void freeSet(const DeviceFunctionTable& functions, DeviceRef device, const DescriptorSet& set);
	};

	class DescriptorPoolSize : public StructBase<VkDescriptorPoolSize, DescriptorPoolSize>
	{
		using Base = StructBase<VkDescriptorPoolSize, DescriptorPoolSize>;
	public:
		using Base::Base;

		DescriptorPoolSize(uint32_t descriptorCount, DescriptorType type) : Base() {
			this->descriptorCount = descriptorCount;
			this->type = convertCEnum(type);
		}

		DescriptorPoolSize& setDescriptorCount(uint32_t descriptorCount) {
			this->descriptorCount = descriptorCount;
			return *this;
		}

		DescriptorPoolSize& setType(DescriptorType type) {
			this->type = convertCEnum(type);
			return *this;
		}
	};

	class DescriptorPoolCreateInfo : public StructBase<VkDescriptorPoolCreateInfo, DescriptorPoolCreateInfo>
	{
		using Base = StructBase<VkDescriptorPoolCreateInfo, DescriptorPoolCreateInfo>;
	public:
		using Base::Base;

		DescriptorPoolCreateInfo(uint32_t maxSets, std::span<const DescriptorPoolSize> poolSizes,
			Flags::DescriptorPoolCreate flags = Flags::DescriptorPoolCreate::Bits::None) : Base() {
			this->maxSets = maxSets;
			this->pPoolSizes = DescriptorPoolSize::underlyingCast(poolSizes.data());
			this->poolSizeCount = poolSizes.size();
			this->flags = flags;
		}

		DescriptorPoolCreateInfo& setMaxSets(uint32_t maxSets) {
			this->maxSets = maxSets;
			return *this;
		}

		DescriptorPoolCreateInfo& setPoolSizes(std::span<const DescriptorPoolSize> poolSizes) {
			this->pPoolSizes = DescriptorPoolSize::underlyingCast(poolSizes.data());
			this->poolSizeCount = poolSizes.size();
			return *this;
		}

		DescriptorPoolCreateInfo& setFlags(Flags::DescriptorPoolCreate flags) {
			this->flags = flags;
			return *this;
		}
	};

	class DescriptorPool : public VerificatorComponent<VkDescriptorPool, DescriptorPoolRef>
	{
		using Base = VerificatorComponent<VkDescriptorPool, DescriptorPoolRef>;
	public:
		using Base::Base;

		void create(const DeviceFunctionTable& functions, DeviceRef device, 
		const DescriptorPoolCreateInfo& createInfo);
		void destroy(const DeviceFunctionTable& functions, DeviceRef device);
	};
}

