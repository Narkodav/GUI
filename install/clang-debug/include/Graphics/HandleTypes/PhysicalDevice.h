#pragma once
#include "Graphics/Common.h"
#include "Graphics/Structs.h"
#include "Graphics/InstanceFunctionTable.h"
#include "Graphics/HandleTypes/Surface.h"
#include "Graphics/HandleTypes/Memory.h"
#include "Graphics/TaskTables/QueuePropertyEnum.h"
#include "Graphics/TaskTables/PropertyEnum.h"
#include "Graphics/TaskTables/FeatureEnum.h"

namespace Graphics
{
    class ExtensionProperties;

    class FormatProperties : public StructBase<VkFormatProperties, FormatProperties>
    {
        using Base = StructBase<VkFormatProperties, FormatProperties>;
    public:
        using Base::Base;

        Flags::FormatFeature getLinearTilingFeatures() const {
            return linearTilingFeatures;
		};

        Flags::FormatFeature getOptimalTilingFeatures() const {
            return optimalTilingFeatures;
        };

        Flags::FormatFeature getBufferFeatures() const {
            return bufferFeatures;
        };
	};

    template <StructureType... types>
    using QueueFamilyPropertyChain = Graphics::StructChain<
        Graphics::StructureType::QueueFamilyProperties2, types...>;

    class CompleteQueueFamilyPropertyChain : public QueueFamilyPropertyChain<
        Graphics::StructureType::QueueFamilyGlobalPriorityPropertiesKHR,
        Graphics::StructureType::QueueFamilyCheckpointPropertiesNV,
        Graphics::StructureType::QueueFamilyCheckpointProperties2NV
    > {
    public:
        using QueueFamilyPropertySetFunc = std::function<void(CompleteQueueFamilyPropertyChain&,
            const std::any&)>;
        static const std::array<QueueFamilyPropertySetFunc,
            static_cast<size_t>(QueueProperty::Num)> s_queuePropertySet;
        using GetQueueFamilyPropertyFunc = std::function<std::any(const CompleteQueueFamilyPropertyChain&)>;
        static const std::array<GetQueueFamilyPropertyFunc,
            static_cast<size_t>(QueueProperty::Num)> s_queuePropertyGet;
        using CheckFunc = std::function<bool(const std::any&, const std::any&)>;
        static const std::array<CheckFunc,
            static_cast<size_t>(QueueProperty::Num)> s_queuePropertyCheck;

        template<QueueProperty property>
        typename QueuePropertyTypeTrait<property>::Type getProperty() const {
            return std::any_cast<QueuePropertyTypeTrait<property>::Type>(s_queuePropertyGet[static_cast<size_t>(property)](*this));
        }

        std::any getProperty(QueueProperty property) const {
            return s_queuePropertyGet[static_cast<size_t>(property)](*this);
        }

        template<QueueProperty property>
        CompleteQueueFamilyPropertyChain& setProperty(typename QueuePropertyTypeTrait<property>::Type data) {
            s_queuePropertySet[static_cast<size_t>(property)](*this, std::any(data));
            return *this;
        }

        CompleteQueueFamilyPropertyChain& setProperty(QueueProperty property, std::any data) {
            s_queuePropertySet[static_cast<size_t>(property)](*this, data);
            return *this;
        }

        bool compare(const CompleteQueueFamilyPropertyChain& requiredChain,
            std::span<const QueueProperty> propToCheck) const {
            for (size_t i = 0; i < propToCheck.size(); ++i)
            {
                std::any required = requiredChain.getProperty(propToCheck[i]);
                std::any availible = getProperty(propToCheck[i]);
                if (!s_queuePropertyCheck[i](required, availible)) return false;
            }
            return true;
        }
    };

    class PhysicalDevice : public BaseComponent<VkPhysicalDevice, PhysicalDevice>
    {
        using Base = BaseComponent<VkPhysicalDevice, PhysicalDevice>;
    public:
        using Base::Base;
        template<StructureType... types>
        using FeatureChain = Graphics::StructChain<
            Graphics::StructureType::PhysicalDeviceFeatures2, types...>;

        template<StructureType... types>
        using PropertyChain = Graphics::StructChain<
            Graphics::StructureType::PhysicalDeviceProperties2, types...>;

        template<StructureType... types>
        using MemoryPropertyChain = Graphics::StructChain<
            Graphics::StructureType::PhysicalDeviceMemoryProperties2, types...>;

        class CompleteFeatureChain : public FeatureChain <
            Graphics::StructureType::PhysicalDeviceVulkan11Features,
            Graphics::StructureType::PhysicalDeviceVulkan12Features,
            Graphics::StructureType::PhysicalDeviceVulkan13Features,
            Graphics::StructureType::PhysicalDeviceMeshShaderFeaturesEXT,
            Graphics::StructureType::PhysicalDeviceRobustness2FeaturesEXT
        > {
        public:

            using FeatureSetFunc = std::function<void(PhysicalDevice::CompleteFeatureChain&, const std::any&)>;
            static const std::array<FeatureSetFunc,
                static_cast<size_t>(DeviceFeature::Num)> s_featureSet;
            using GetFeatureFunc = std::function<std::any(const PhysicalDevice::CompleteFeatureChain&)>;
            static const std::array<GetFeatureFunc,
                static_cast<size_t>(DeviceFeature::Num)> s_featureGet;
            using CheckFunc = std::function<bool(const std::any&, const std::any&)>;
            static const std::array<CheckFunc,
                static_cast<size_t>(DeviceFeature::Num)> s_featureCheck;


            template<DeviceFeature feature>
            typename DeviceFeatureTypeTrait<feature>::Type getFeature() const {
                return std::any_cast<DeviceFeatureTypeTrait<feature>::Type>(s_featureGet[static_cast<size_t>(feature)](*this));
            }

            std::any getFeature(DeviceFeature feature) const {
                return s_featureGet[static_cast<size_t>(feature)](*this);
            }

            template<DeviceFeature feature>
            CompleteFeatureChain& setFeature(typename DeviceFeatureTypeTrait<feature>::Type data) {
                s_featureSet[static_cast<size_t>(feature)](*this, std::any(data));
                return *this;
            }

            CompleteFeatureChain& setFeature(DeviceFeature feature, std::any data) {
                s_featureSet[static_cast<size_t>(feature)](*this, data);
                return *this;
            }

            bool compare(const CompleteFeatureChain& requiredChain,
                std::span<const DeviceFeature> featureToCheck) const {
                for (size_t i = 0; i < featureToCheck.size(); ++i)
                {
                    std::any required = requiredChain.getFeature(featureToCheck[i]);
                    std::any availible = getFeature(featureToCheck[i]);
                    if (!s_featureCheck[i](required, availible)) return false;
                }
                return true;
            }
        };

        class CompletePropertyChain : public PropertyChain<
            Graphics::StructureType::PhysicalDeviceVulkan11Properties,
            Graphics::StructureType::PhysicalDeviceVulkan12Properties,
            Graphics::StructureType::PhysicalDeviceVulkan13Properties,
            Graphics::StructureType::PhysicalDeviceMultiviewProperties,
            Graphics::StructureType::PhysicalDeviceProtectedMemoryProperties,
            Graphics::StructureType::PhysicalDeviceFloatControlsProperties,
            Graphics::StructureType::PhysicalDeviceDescriptorIndexingProperties,
            Graphics::StructureType::PhysicalDeviceTimelineSemaphoreProperties,
            Graphics::StructureType::PhysicalDeviceInlineUniformBlockProperties,
            Graphics::StructureType::PhysicalDeviceSubgroupSizeControlProperties,
            Graphics::StructureType::PhysicalDeviceShaderIntegerDotProductProperties,
            Graphics::StructureType::PhysicalDeviceMaintenance4Properties
        > {
        public:
            using PropertySetFunc = std::function<void(PhysicalDevice::CompletePropertyChain&, const std::any&)>;
            static const std::array<PropertySetFunc,
                static_cast<size_t>(DeviceProperty::Num)> s_propertySet;
            using GetPropertyFunc = std::function<std::any(const PhysicalDevice::CompletePropertyChain&)>;
            static const std::array<GetPropertyFunc,
                static_cast<size_t>(DeviceProperty::Num)> s_propertyGet;
            using CheckFunc = std::function<bool(const std::any&, const std::any&)>;
            static const std::array<CheckFunc,
                static_cast<size_t>(DeviceProperty::Num)> s_propertyCheck;

            template<DeviceProperty property>
            typename DevicePropertyTypeTrait<property>::Type getProperty() const {
                return std::any_cast<DevicePropertyTypeTrait<property>::Type>(s_propertyGet[static_cast<size_t>(property)](*this));
            }

            std::any getProperty(DeviceProperty property) const {
                return s_propertyGet[static_cast<size_t>(property)](*this);
            }

            template<DeviceProperty property>
            CompletePropertyChain& setProperty(typename DevicePropertyTypeTrait<property>::Type data) {
                s_propertySet[static_cast<size_t>(property)](*this, std::any(data));
                return *this;
            }

            CompletePropertyChain& setProperty(DeviceProperty property, std::any data) {
                s_propertySet[static_cast<size_t>(property)](*this, data);
                return *this;
            }

            bool compare(const CompletePropertyChain& requiredChain,
                std::span<const DeviceProperty> propsToCheck) const {
                for (size_t i = 0; i < propsToCheck.size(); ++i)
                {
                    std::any required = requiredChain.getProperty(propsToCheck[i]);
                    std::any availible = getProperty(propsToCheck[i]);
                    if (!s_propertyCheck[i](required, availible)) return false;
                }
                return true;
            }
        };

        //class CompleteMemoryPropertyChain {
        //    MemoryPropertyChain<
        //    > m_chain;


        //public:
        //};

        PhysicalDeviceMemoryProperties getMemoryProperties(const InstanceFunctionTable& functions) const;

        SurfaceCapabilities getSurfaceCapabilities(const InstanceFunctionTable& functions,
            SurfaceRef surface) const;
        std::vector<SurfaceFormat> getSurfaceFormats(const InstanceFunctionTable& functions,
            SurfaceRef surface) const;
        std::vector<PresentMode> getSurfacePresentModes(const InstanceFunctionTable& functions,
            SurfaceRef surface) const;

        FormatProperties getFormatProperties(const InstanceFunctionTable& functions, Format format) const;

        std::vector<ExtensionProperties> getExtensions(const InstanceFunctionTable& functions) const;

        bool getSurfaceSupport(const InstanceFunctionTable& functions, SurfaceRef surface, uint32_t familyIndex) const;

        template<typename QueuePropertyVector>
        void getQueueFamilyProperties(const InstanceFunctionTable& functions,
            QueuePropertyVector& queueProperties) const
        {
            uint32_t queueFamilyCount = 0;

            functions.execute<InstanceFunction::GetPhysicalDeviceQueueFamilyProperties2>(
                getHandle(), &queueFamilyCount, nullptr);
            queueProperties.resize(queueFamilyCount);
            std::vector<VkQueueFamilyProperties2> temporary(queueFamilyCount);
            for (size_t i = 0; i < temporary.size(); ++i)
                temporary[i] = queueProperties[i].getHead();

            functions.execute<InstanceFunction::GetPhysicalDeviceQueueFamilyProperties2>(
                getHandle(), &queueFamilyCount, temporary.data());

            for (size_t i = 0; i < queueProperties.size(); ++i)
                queueProperties[i].getHead() = temporary[i];
        }

        template<StructureType... types>
        void getFeatures(const InstanceFunctionTable& functions, FeatureChain<types...>& chain) const
        {
            chain = getQualitiesImpl<FeatureChain<types...>,
                InstanceFunction::GetPhysicalDeviceFeatures2,
                VkPhysicalDeviceFeatures2>(functions);
        }

        template<StructureType... types>
        void getProperties(const InstanceFunctionTable& functions, PropertyChain<types...>& chain) const
        {
            chain = getQualitiesImpl<PropertyChain<types...>,
                InstanceFunction::GetPhysicalDeviceProperties2,
                VkPhysicalDeviceProperties2>(functions);
        }

        template<StructureType... types>
        void getMemoryProperties(const InstanceFunctionTable& functions, MemoryPropertyChain<types...>& chain) const
        {
            chain = getQualitiesImpl<MemoryPropertyChain<types...>,
                InstanceFunction::GetPhysicalDeviceMemoryProperties2,
                VkPhysicalDeviceMemoryProperties2>(functions);
        }

    private:
        template<typename ChainType, InstanceFunction function, typename CastType>
        ChainType getQualitiesImpl(const InstanceFunctionTable& functions) const
        {
            ChainType chain;
            functions.execute<function>(getHandle(), reinterpret_cast<CastType*>(&chain.getHead()));
            return chain;
        }
    };
}

