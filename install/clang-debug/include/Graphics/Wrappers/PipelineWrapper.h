#pragma once
#include <mutex>

#include "Graphics/HandleTypes/Pipeline.h"
#include "Graphics/HandleTypes/CommandBuffer.h"
#include "Graphics/Wrappers/DeviceWrapper.h"

namespace Graphics::Wrappers {

    class Pipeline {
    private:
        Graphics::Pipeline m_pipeline;
        PipelineLayoutRef m_layout;

    public:
        inline Pipeline() = default;

        inline Pipeline(const Device& device, const PipelineCreateInfo& createInfo) {
            create(device, createInfo);
        }

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
        Pipeline(Pipeline&&) = default;
        Pipeline& operator=(Pipeline&&) = default;

        inline PipelineRef getReference() const { return m_pipeline; }
        inline operator PipelineRef() const { return getReference(); }

        inline VkPipeline getHandle() const { return m_pipeline.getHandle(); }

        inline PipelineLayoutRef getLayout() const { return m_layout; }

        inline void create(const Device& device, const PipelineCreateInfo& createInfo) {
            m_pipeline.create(device.getFunctionTable(), device, createInfo);
        }

        inline void destroy(const Device& device) {
            if (m_pipeline.isValid()) {
                m_pipeline.destroy(device.getFunctionTable(), device);
            }
        }

        inline void bind(
            const Device& device,
            CommandBuffer cmd,
            PipelineBindPoint bindPoint
        ) const {
            cmd.bindPipeline(device.getFunctionTable(), m_pipeline, bindPoint);
        }
    };
}
