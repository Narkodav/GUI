#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"

namespace Graphics
{
    class DeviceRef;

    class ShaderModuleRef : public BaseComponent<VkShaderModule, ShaderModuleRef>
    {
        using Base = BaseComponent<VkShaderModule, ShaderModuleRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "ShaderModule";
    };

    class ShaderModuleCreateInfo : public StructBase<VkShaderModuleCreateInfo, ShaderModuleCreateInfo>
    {
        using Base = StructBase<VkShaderModuleCreateInfo, ShaderModuleCreateInfo>;
    public:
        using Base::Base;

        ShaderModuleCreateInfo(std::span<const char> shaderCode) : Base()
        {
            this->flags = 0;
            this->codeSize = shaderCode.size();
            this->pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
        }

        ShaderModuleCreateInfo& setShaderCode(std::span<const char> shaderCode) {
            this->codeSize = shaderCode.size();
            this->pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
            return *this;
        }
    };

    class ShaderModule : public VerificatorComponent<VkShaderModule, ShaderModuleRef>
    {
        using Base = VerificatorComponent<VkShaderModule, ShaderModuleRef>;
    public:
        using Base::Base;

        void create(const DeviceFunctionTable& functions, const DeviceRef& device, const ShaderModuleCreateInfo& createInfo);
        void destroy(const DeviceFunctionTable& functions, const DeviceRef& device);

        static std::vector<char> parseShaderCodeSPIRV(std::string_view filename);
        static Flags::ShaderStage::Bits inferShaderStage(std::span<const char> spirvCode);
    };
}

