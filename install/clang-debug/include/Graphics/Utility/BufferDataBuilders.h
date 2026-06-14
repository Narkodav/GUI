#pragma once
#include "Graphics/Common.h"
#include "Graphics/HandleTypes/Pipeline.h"

namespace Graphics::Utility {

    template <typename... Types>
    class TypeErasedArray
    {
    protected:
        std::tuple<Types...> m_data;

    public:
        template<typename Func>
        constexpr void iterate(Func&& callback) {
            (... && callback(std::get<Types>(m_data)));
        }

        template<typename Func>
        constexpr void iterate(Func&& callback) const {
            (... && callback(std::get<Types>(m_data)));
        }

        template<typename Func>
        constexpr void iterateComplete(Func&& callback) {
            (callback(std::get<Types>(m_data)), ...);
        }

        template<typename Func>
        constexpr void iterateComplete(Func&& callback) const {
            (callback(std::get<Types>(m_data)), ...);
        }

        template<size_t I>
        constexpr auto& get() { return std::get<I>(m_data); }

        template<size_t I>
        constexpr const auto& get() const { return std::get<I>(m_data); }

        constexpr static size_t size() { return sizeof...(Types); };
        constexpr static bool empty() { return sizeof...(Types) == 0; };
    };

    template<typename T>
    concept VertexDefinition = requires {
        { T::getBindingDescriptions() } -> std::same_as<std::span<VertexInputBindingDescription>>;
        { T::getAttributeDescriptions() } -> std::same_as<std::span<VertexInputAttributeDescription>>;
    };

    template<typename T>
    concept DescriptorDefinition = requires {
        { T::getDescriptorSetLayoutBinding() } -> std::same_as<DescriptorSetLayoutBinding>;
        { T::getDescriptorBindingFlags() } -> std::same_as<Flags::DescriptorBinding>;
    };

    template<typename T>
    concept VertexDefinitionDerived = requires {
        { T::s_bindings + 0 } -> std::convertible_to<const VertexInputBindingDescription*>;
        { T::s_attributes + 0 } -> std::convertible_to<const VertexInputAttributeDescription*>;

        { T::s_bindingCount } -> std::convertible_to<size_t>;
        { T::s_attributeCount } -> std::convertible_to<size_t>;
    };

    template<typename T>
    concept DescriptorDefinitionDerived = requires {
        // Check s_descriptorBindingFlags is an array of DescriptorSetLayoutBinding
        requires std::same_as<
            std::remove_extent_t<decltype(T::s_descriptorBindingFlags)>,
                DescriptorSetLayoutBinding
        >;
        // Assuming you meant s_descriptorFlags for the second one
        requires std::same_as<
            std::remove_extent_t<decltype(T::s_descriptorFlags)>,  // Different member!
                Flags::DescriptorBinding
        >;
    };

    template <VertexDefinition... Defs>
    class VertexDefinitionBuilder : public TypeErasedArray<Defs...> {
    private:
        //template<VertexDefinition... Defs>
        //consteval bool bindingsAreUnique()
        //{
        //    std::array<uint32_t, sizeof...(Defs)> bindings = {
        //        Defs::getBindingDescriptions()[0].binding...
        //    };

        //    for (size_t i = 0; i < bindings.size(); ++i)
        //        for (size_t j = i + 1; j < bindings.size(); ++j)
        //            if (bindings[i] == bindings[j])
        //                return false;

        //    return true;
        //}

        //static_assert(bindingsAreUnique<Defs...>());

    private:
    consteval size_t getAttributeCount() {
        size_t count = 0;
        this->iterateComplete([&]<VertexDefinition T>(const T & elem) {
            const auto& attributes = elem.getAttributeDescriptions();
            count += attributes.size();
        });
        return count;
    }

    consteval size_t getBindingCount() {
        size_t count = 0;
        this->iterateComplete([&]<VertexDefinition T>(const T & elem) {
            const auto& bindings = elem.getBindingDescriptions();
            count += bindings.size();
        });
        return count;
    }

    public:
        consteval auto enumerateAttributes() const
        {
            std::array<VertexInputAttributeDescription, getAttributeCount()> attributeDescriptions;
            size_t index = 0;
            this->iterateComplete([&]<VertexDefinition T>(const T & elem) {
                const auto& attributes = elem.getAttributeDescriptions();
                for (int i = 0; i < attributes.size(); ++i, ++index)
                    attributeDescriptions[index] = attributes[i];
            });
            return attributeDescriptions;
        }

        consteval auto enumerateBindings() const
        {
            std::array<VertexInputBindingDescription, getBindingCount()> bindingDescriptions;
            size_t index = 0;
            this->iterateComplete([&]<VertexDefinition T>(const T & elem) {
                const auto& bindings = elem.getBindingDescriptions();
                for (int i = 0; i < bindings.size(); ++i, ++index)
                    bindingDescriptions[index] = bindings[i];
            });
            return bindingDescriptions;
        }
    };

    template <DescriptorDefinition... Defs>
    class DescriptorDefinitionBuilder : public TypeErasedArray<Defs...> {
    public:
        constexpr auto enumerateDescriptors() const
        {
            std::array<DescriptorSetLayoutBinding, sizeof...(Defs)> descriptors;
            size_t index = 0;
            this->iterateComplete([&]<DescriptorDefinition T>(const T & elem) {
                descriptors[index++] = elem.getDescriptorSetLayoutBinding();
            });
            return descriptors;
        }

        constexpr auto enumerateBindingFlags() const
        {
            std::array<Flags::DescriptorBinding, this->size()> bindingFlags;
            size_t index = 0;
            this->iterateComplete([&]<DescriptorDefinition T>(const T & elem) {
                bindingFlags[index++] = elem.getDescriptorBindingFlags();
            });
            return bindingFlags;
        }
    };

    template<typename Derived>
    class VertexDefinitionBase {
    public:
        static std::span<const VertexInputBindingDescription> getBindingDescriptions() {
            return std::span<const VertexInputBindingDescription>(Derived::s_bindings, Derived::s_bindingCount);
        }

        static std::span<const VertexInputAttributeDescription> getAttributeDescriptions() {
            return std::span<const VertexInputAttributeDescription>(Derived::s_attributes, Derived::s_attributeCount);
        }
    };

    struct VertexBasic
    {
        glm::vec4 position;
        glm::vec4 UV;
    };

    struct VertexDefinitionBasic : public VertexDefinitionBase<VertexDefinitionBasic> {
    public:
        using Type = VertexBasic;

        static inline const size_t s_bindingCount = 1;
        static inline const size_t s_attributeCount = 2;
        static inline const size_t s_dataSize = sizeof(Type);

        static inline const VertexInputBindingDescription s_bindings[] = {
            {
                0,                                      // binding
                s_dataSize,                             // stride
                VertexInputRate::Vertex                 // input rate
            }
        };

        static inline const VertexInputAttributeDescription s_attributes[] = {
            {
                0,                                      // location
                0,                                      // binding
                Format::R32G32B32A32Sfloat,        // format
                0                                       // offset
            },
            {
                1,                                      // location
                0,                                      // binding
                Format::R32G32B32A32Sfloat,        // format
                sizeof(glm::vec4)                       // offset
            }
        };
    };

    struct VertexDefinitionPosition : public VertexDefinitionBase<VertexDefinitionPosition> {
    public:
        using Type = glm::vec4;

        static constexpr size_t s_bindingCount = 1;
        static constexpr size_t s_attributeCount = 1;
        static constexpr size_t s_dataSize = sizeof(Type);

        static inline const VertexInputBindingDescription s_bindings[] = {
            {
                0,                                      // binding
                s_dataSize,                             // stride
                VertexInputRate::Vertex                 // input rate
            }
        };

        static inline const VertexInputAttributeDescription s_attributes[] = {
            {
                0,                                      // location
                0,                                      // binding
                Format::R32G32B32A32Sfloat,        // format
                0                                       // offset
            }
        };
    };

    struct VertexDefinitionUV : public VertexDefinitionBase<VertexDefinitionUV> {
    public:
        using Type = glm::vec2;

        static constexpr size_t s_bindingCount = 1;
        static constexpr size_t s_attributeCount = 1;
        static constexpr size_t s_dataSize = sizeof(Type);

        static constexpr VertexInputBindingDescription s_bindings[] = {
            {
                1,                                      // binding
                s_dataSize,                             // stride
                VertexInputRate::Vertex                 // input rate
            }
        };

        static constexpr VertexInputAttributeDescription s_attributes[] = {
            {
                1,                                      // location
                1,                                      // binding
                Format::R32G32Sfloat,              // format
                0                                       // offset
            }
        };
    };

    struct ModelTransform
    {
        glm::mat4 transform;
    };

    struct VertexDefinitionModelTransform : public VertexDefinitionBase<VertexDefinitionModelTransform> {
    public:
        using Type = glm::mat4;

        static constexpr size_t s_bindingCount = 1;
        static constexpr size_t s_attributeCount = 4;
        static constexpr size_t s_dataSize = sizeof(Type);

        static constexpr VertexInputBindingDescription s_bindings[] = {
            {
                2,                                      // binding
                s_dataSize,                             // stride
                VertexInputRate::Instance               // input rate
            }
        };

        static constexpr VertexInputAttributeDescription s_attributes[] = {
            {
                2,                                      // location
                2,                                      // binding
                Format::R32G32B32A32Sfloat,        // format
                0                                       // offset
            },
            {
                3,                                      // location
                2,                                      // binding
                Format::R32G32B32A32Sfloat,        // format
                sizeof(glm::vec4)                       // offset
            },
            {
                4,                                      // location
                2,                                      // binding
                Format::R32G32B32A32Sfloat,        // format
                sizeof(glm::vec4) * 2                   // offset
            },
            {
                5,                                      // location
                2,                                      // binding
                Format::R32G32B32A32Sfloat,        // format
                sizeof(glm::vec4) * 3                   // offset
            }
        };
    };

    struct VertexDefinitionId : public VertexDefinitionBase<VertexDefinitionModelTransform> {
    public:
        using Type = uint32_t;

        static constexpr size_t s_bindingCount = 1;
        static constexpr size_t s_attributeCount = 1;
        static constexpr size_t s_dataSize = sizeof(Type);

        static constexpr VertexInputBindingDescription s_bindings[] = {
            {
                3,                                      // binding
                s_dataSize,                             // stride
                VertexInputRate::Instance               // input rate
            }
        };

        static constexpr VertexInputAttributeDescription s_attributes[] = {
            {
                6,                                      // location
                3,                                      // binding
                Format::R32Uint,                   // format
                0                                       // offset
            },
        };
    };

    struct UniformTransforms {
        glm::mat4 view;
        glm::mat4 proj;
    };

    template<typename Derived>
    class DescriptorDefinitionBase {
    public:
        static DescriptorSetLayoutBinding
            getDescriptorSetLayoutBinding()
        {
            return Derived::s_descriptorSetLayoutBinding;
        };

        static Flags::DescriptorBinding getDescriptorBindingFlags()
        {
            return Derived::s_descriptorBindingFlags;
        };
    };

    //immutable samplers require runtime definition

    struct UniformTransformsDefinition : public DescriptorDefinitionBase<UniformTransformsDefinition> {
        static constexpr DescriptorSetLayoutBinding s_descriptorSetLayoutBinding = {
                0,                                      // binding
                DescriptorType::UniformBuffer,          // descriptor type
                1,                                      // descriptor count
                Flags::ShaderStage::Bits::Vertex,       // stage flags
        };

        static constexpr Flags::DescriptorBinding s_descriptorBindingFlags = Flags::DescriptorBinding::Bits::None;
    };

    struct ImageSamplerDefinition : public DescriptorDefinitionBase<ImageSamplerDefinition> {
        static constexpr DescriptorSetLayoutBinding s_descriptorSetLayoutBinding = {
                1,                                              // binding
                DescriptorType::CombinedImageSampler,           // descriptor type
                1,                                              // descriptor count
                Flags::ShaderStage::Bits::Fragment,             // stage flags
        };

        static constexpr Flags::DescriptorBinding s_descriptorBindingFlags = Flags::DescriptorBinding::Bits::None;
    };

    template <size_t binding>
    struct BindlessImageSamplerDefinition : public DescriptorDefinitionBase<BindlessImageSamplerDefinition<binding>> {
        static constexpr DescriptorSetLayoutBinding s_descriptorSetLayoutBinding = {
                binding,                                        // binding
                DescriptorType::CombinedImageSampler,           // descriptor type
                1024,                                           // descriptor count
                Flags::ShaderStage::Bits::Fragment,             // stage flags
        };

        static constexpr Flags::DescriptorBinding s_descriptorBindingFlags = Flags::DescriptorBinding::Bits::None;
    };
}