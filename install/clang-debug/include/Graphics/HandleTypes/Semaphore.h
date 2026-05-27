#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"

namespace Graphics
{
	class DeviceRef;

	class SemaphoreRef : public BaseComponent<VkSemaphore, SemaphoreRef>
	{
		using Base = BaseComponent<VkSemaphore, SemaphoreRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "Semaphore";
	};

	class SemaphoreCreateInfo : public StructBase<VkSemaphoreCreateInfo, SemaphoreCreateInfo>
	{
		using Base = StructBase<VkSemaphoreCreateInfo, SemaphoreCreateInfo>;
	public:
		using Base::Base;
		SemaphoreCreateInfo(Flags::SemaphoreCreate flags = Flags::SemaphoreCreate::Bits::None) : Base() {
			this->flags = flags;
		}
		SemaphoreCreateInfo& setFlags(Flags::SemaphoreCreate flags) {
			this->flags = flags;
			return *this;
		}
		Flags::SemaphoreCreate getFlags() const {
			return this->flags;
		}
	};

    class Semaphore : public VerificatorComponent<VkSemaphore, SemaphoreRef>
    {
        using Base = VerificatorComponent<VkSemaphore, SemaphoreRef>;
    public:
		using Base::Base;

		void create(const DeviceFunctionTable& functions, DeviceRef device,
			const SemaphoreCreateInfo& createInfo = SemaphoreCreateInfo()); // SemaphoreCreateInfo is currently useless, as there are no flags defined
        void destroy(const DeviceFunctionTable& functions, DeviceRef device);
    };
}
