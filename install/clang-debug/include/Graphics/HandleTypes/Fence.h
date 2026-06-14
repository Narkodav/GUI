#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"

namespace Graphics
{
    class Fence;
    
    class FenceRef : public BaseComponent<VkFence, FenceRef>
    {
        using Base = BaseComponent<VkFence, FenceRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "Fence";

        void wait(const DeviceFunctionTable& functions, DeviceRef device,
            size_t timeout = std::numeric_limits<size_t>::max()) const;

        void reset(const DeviceFunctionTable& functions, DeviceRef device);

        template<ContiguousContainer Container>
        static void reset(const DeviceFunctionTable& functions, DeviceRef device,
            const Container& fences) requires
            std::same_as<typename Container::value_type, FenceRef> ||
            std::same_as<typename Container::value_type, Fence>
        {
            reset(functions, device, FenceRef::underlyingCast(fences.data()), fences.size());
        }

        static void reset(const DeviceFunctionTable& functions, DeviceRef device,
            const FenceRef* fences, size_t count);

        template<ContiguousContainer Container>
        static void wait(const DeviceFunctionTable& functions, DeviceRef device,
            Container fences, size_t timeout = std::numeric_limits<size_t>::max(),
            bool waitAll = true) requires 
            std::same_as<typename Container::value_type, FenceRef> ||
            std::same_as<typename Container::value_type, Fence>
        {
            wait(functions, device, fences.data(), fences.size(), timeout, waitAll);
        }

        static void wait(const DeviceFunctionTable& functions, DeviceRef device,
            const FenceRef* fences, size_t count, size_t timeout = std::numeric_limits<size_t>::max(),
            bool waitAll = true);
    };

    class FenceCreateInfo : public StructBase<VkFenceCreateInfo, FenceCreateInfo>
    {
        using Base = StructBase<VkFenceCreateInfo, FenceCreateInfo>;
    public:
        using Base::Base;
        FenceCreateInfo(Flags::FenceCreate flags = Flags::FenceCreate::Bits::Signaled) : Base() {
            this->flags = flags;
        }

        FenceCreateInfo& setFlags(Flags::FenceCreate flags) {
            this->flags = flags;
            return *this;
        }

        Flags::FenceCreate getFlags() const {
            return this->flags;
        }
    };

    class Fence : public VerificatorComponent<VkFence, FenceRef>
    {
        using Base = VerificatorComponent<VkFence, FenceRef>;
    public:
        using Base::Base;        

        void create(const DeviceFunctionTable& functions, DeviceRef device,
            const FenceCreateInfo& createInfo = FenceCreateInfo());

        void destroy(const DeviceFunctionTable& functions, DeviceRef device);
    };
}



