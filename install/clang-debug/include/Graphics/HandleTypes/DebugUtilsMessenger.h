#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../FunctionPointers.h"
#include "../InstanceFunctionTable.h"

namespace Graphics {
    class InstanceRef;
};

namespace Graphics::DebugUtils
{
    class ObjectNameInfo : public StructBase<VkDebugUtilsObjectNameInfoEXT, ObjectNameInfo>
    {
        using Base = StructBase<VkDebugUtilsObjectNameInfoEXT, ObjectNameInfo>;
    public:
        using Base::Base;
        bool hasObjectName() const { return this->pObjectName != nullptr; }
        std::string_view getName() const { return std::string_view(this->pObjectName); };
        ObjectType getType() const { return convertCEnum(this->objectType); };
        uint64_t getHandle() const { return this->objectHandle; };
    };

    class Label : public StructBase<VkDebugUtilsLabelEXT, Label>
    {
        using Base = StructBase<VkDebugUtilsLabelEXT, Label>;
    public:
        using Base::Base;
        const Color& getColor() const { return *reinterpret_cast<const Color*>(this->color); };
        std::string_view getName() const { return this->pLabelName; };
    };

    class CallbackData : public StructBase<VkDebugUtilsMessengerCallbackDataEXT, CallbackData>
    {
        using Base = StructBase<VkDebugUtilsMessengerCallbackDataEXT, CallbackData>;
    public:
        using Base::Base;
        const std::string_view getMessageIdName() const { return this->pMessageIdName; };
        const std::string_view getMessage() const { return this->pMessage; };
        uint32_t getMessageIdNumber() const { return this->messageIdNumber; };

        std::span<const ObjectNameInfo> getObjectInfos() const {
            return std::span<const ObjectNameInfo>(
                ObjectNameInfo::underlyingCast(this->pObjects),
                this->objectCount);
        };

        std::span<const Label> getQueueLabels() const {
            return std::span<const Label>(
                Label::underlyingCast(this->pQueueLabels),
                this->queueLabelCount);
        };

        std::span<const Label> getCmdBufLabels() const {
            return std::span<const Label>(
                Label::underlyingCast(this->pCmdBufLabels),
                this->cmdBufLabelCount);
        };

        const Flags::DebugUtils::MessengerCallbackData& getFlags() const
        {
            return *reinterpret_cast<const Flags::DebugUtils::MessengerCallbackData*>(&this->flags);
        };
    };

    class MessengerRef : public BaseComponent<VkDebugUtilsMessengerEXT, MessengerRef> {
        using Base = BaseComponent<VkDebugUtilsMessengerEXT, MessengerRef>;
    public:        
		using Base::Base;
        static inline const std::string s_typeName = "Messenger";
    };

    class MessengerCreateInfo : public StructBase<VkDebugUtilsMessengerCreateInfoEXT, MessengerCreateInfo>
    {
        using Base = StructBase<VkDebugUtilsMessengerCreateInfoEXT, MessengerCreateInfo>;
    public:
        using Base::Base;

        MessengerCreateInfo(PFN_callback callback,
            Flags::DebugMessageSeverity messageSeverityFlags = Flags::DebugMessageSeverity::Bits::All,
            Flags::DebugMessageType messageTypeFlags = Flags::DebugMessageType::Bits::AllNoAddressBinding,
            void* pUserData = nullptr) : Base() {
            this->messageSeverity = messageSeverityFlags;
            this->messageType = messageTypeFlags;
            this->pfnUserCallback = reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(reinterpret_cast<void*>(callback));
            this->pUserData = pUserData;
        };

        MessengerCreateInfo& setCallback(PFN_callback callback) {
            this->pfnUserCallback = reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(reinterpret_cast<void*>(callback));
            return *this;
        }

        MessengerCreateInfo& setMessageSeverity(Flags::DebugMessageSeverity messageSeverityFlags) {
            this->messageSeverity = messageSeverityFlags;
            return *this;
        }

        MessengerCreateInfo& setMessageType(Flags::DebugMessageType messageTypeFlags) {
            this->messageType = messageTypeFlags;
            return *this;
        }

        MessengerCreateInfo& setUserData(void* pUserData) {
            this->pUserData = pUserData;
            return *this;
        }

        PFN_callback getCallback() const {
            return reinterpret_cast<PFN_callback>(reinterpret_cast<void*>(this->pfnUserCallback));
        }

        Flags::DebugMessageSeverity getMessageSeverity() const {
            return this->messageSeverity;
        }

        Flags::DebugMessageType getMessageType() const {
            return this->messageType;
        }

        void* getUserData() const {
            return this->pUserData;
        }
    };

    class Messenger : public VerificatorComponent<VkDebugUtilsMessengerEXT, MessengerRef>
    {
        using Base = VerificatorComponent<VkDebugUtilsMessengerEXT, MessengerRef>;
    public:
        using Base::Base;        

        void create(const InstanceFunctionTable& functions, InstanceRef instance,
            const MessengerCreateInfo& debugCreateInfo);

        void destroy(const InstanceFunctionTable& functions, InstanceRef instance);
    };

}