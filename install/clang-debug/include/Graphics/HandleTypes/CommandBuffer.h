#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "SwapChain.h"
#include "Pipeline.h"
#include "FrameBuffer.h"
#include "Buffer.h"
#include "Memory.h"
#include "Image.h"

namespace Graphics
{
    class RenderPassBeginInfo : public StructBase<VkRenderPassBeginInfo, RenderPassBeginInfo>
    {
        using Base = StructBase<VkRenderPassBeginInfo, RenderPassBeginInfo>;
    public:
        using Base::Base;

        RenderPassBeginInfo(RenderPassRef renderPass, FrameBufferRef frameBuffer,
            std::span<const ClearValue> clearValues, const Rect2D& renderArea) : Base()
        {
            this->renderPass = renderPass.getHandle();
            this->framebuffer = frameBuffer.getHandle();
            this->clearValueCount = clearValues.size();
            this->pClearValues = ClearValue::underlyingCast(clearValues.data());
            this->renderArea = renderArea;
        }

        RenderPassBeginInfo(RenderPassRef renderPass, FrameBufferRef frameBuffer,
            std::span<const ClearValue> clearValues, const Offset2D& offset,
            const Extent2D& extent) : Base()
        {
            this->renderPass = renderPass.getHandle();
            this->framebuffer = frameBuffer.getHandle();
            this->clearValueCount = clearValues.size();
            this->pClearValues = ClearValue::underlyingCast(clearValues.data());
            this->renderArea.offset = offset;
            this->renderArea.extent = extent;
        }

        RenderPassBeginInfo& setRenderPass(RenderPassRef renderPass) {
            this->renderPass = renderPass.getHandle();
            return *this;
        }

        RenderPassBeginInfo& setFrameBuffer(FrameBufferRef frameBuffer) {
            this->framebuffer = frameBuffer.getHandle();
            return *this;
        }

        RenderPassBeginInfo& setClearValues(std::span<const ClearValue> clearValues) {
            this->clearValueCount = clearValues.size();
            this->pClearValues = ClearValue::underlyingCast(clearValues.data());
            return *this;
        }

        RenderPassBeginInfo& setClearValue(const ClearValue& clearValue) {
            this->clearValueCount = 1;
            this->pClearValues = clearValue.getUnderlyingPointer();
            return *this;
        }

        RenderPassBeginInfo& setRenderArea(const Rect2D& renderArea) {
            this->renderArea = renderArea;
            return *this;
        }

        RenderPassBeginInfo& setRenderArea(const Offset2D& offset, const Extent2D& extent) {
            this->renderArea.offset = offset;
            this->renderArea.extent = extent;
            return *this;
        }
    };

    class CommandBufferInheritanceInfo : public StructBase<VkCommandBufferInheritanceInfo, CommandBufferInheritanceInfo>
    {
        using Base = StructBase<VkCommandBufferInheritanceInfo, CommandBufferInheritanceInfo>;
    public:
        using Base::Base;

        CommandBufferInheritanceInfo(const RenderPassRef& renderPass,
            uint32_t subpass, const FrameBuffer& framebuffer,
            bool occlusionQueryEnable, Flags::QueryControl queryControl,
            Flags::QueryPipelineStatistic queryPipelineStatistic) : Base()
        {
            this->renderPass = renderPass.getHandle();
            this->subpass = subpass;
            this->framebuffer = framebuffer;
            this->occlusionQueryEnable = occlusionQueryEnable;
            this->queryFlags = queryControl;
            this->pipelineStatistics = queryPipelineStatistic;
        }

        CommandBufferInheritanceInfo& setRenderPass(RenderPassRef renderPass)
        {
            this->renderPass = renderPass.getHandle();
            return *this;
        }

        CommandBufferInheritanceInfo& setSubpass(uint32_t subpass)
        {
            this->subpass = subpass;
            return *this;
        }

        CommandBufferInheritanceInfo& setFrameBuffer(const FrameBuffer& framebuffer)
        {
            this->framebuffer = framebuffer;
            return *this;
        }

        CommandBufferInheritanceInfo& setOcclusionQueryEnable(bool occlusionQueryEnable)
        {
            this->occlusionQueryEnable = occlusionQueryEnable;
            return *this;
        }

        CommandBufferInheritanceInfo& setQueryFlags(Flags::QueryControl flags)
        {
            this->queryFlags = flags;
            return *this;
        }

        CommandBufferInheritanceInfo& setPipelineStatistic(Flags::QueryPipelineStatistic flags)
        {
            this->pipelineStatistics = flags;
            return *this;
        }
    };

    class CommandBufferBeginInfo : public StructBase<VkCommandBufferBeginInfo, CommandBufferBeginInfo>
    {
        using Base = StructBase<VkCommandBufferBeginInfo, CommandBufferBeginInfo>;
    public:
        using Base::Base;

        CommandBufferBeginInfo(Flags::CommandBufferUsage flags = Flags::CommandBufferUsage::Bits::None) : Base()
        {
            this->flags = flags;
        }

        CommandBufferBeginInfo(const CommandBufferInheritanceInfo& inheritanceInfo,
            Flags::CommandBufferUsage flags = Flags::CommandBufferUsage::Bits::None) : Base()
        {
            this->pInheritanceInfo = CommandBufferInheritanceInfo::underlyingCast(&inheritanceInfo);
            this->flags = flags;
        }

        CommandBufferBeginInfo& setFlags(Flags::CommandBufferUsage flags)
        {
            this->flags = flags;
            return *this;
        }

        CommandBufferBeginInfo& setInheritanceInfo(const CommandBufferInheritanceInfo& inheritanceInfo)
        {
            this->pInheritanceInfo = CommandBufferInheritanceInfo::underlyingCast(&inheritanceInfo);
            return *this;
        }
    };

	class CommandPool;
    class CommandPoolRef;

	class CommandBuffer : public BaseComponent<VkCommandBuffer, CommandBuffer>
	{
        friend class CommandPool;
        friend class CommandPoolRef;
		using Base = BaseComponent<VkCommandBuffer, CommandBuffer>;
	public:
		using Base::Base;        

        template<typename Func>
        Result record(const DeviceFunctionTable& functions, const CommandBufferBeginInfo& beginInfo, Func&& func)
        {
            GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
            Result result = begin(functions, beginInfo);
            func(result, *this);
            return end(functions);
        };

        void beginRenderPass(const DeviceFunctionTable& functions, const RenderPassBeginInfo& beginInfo,
            SubpassContents subpassContents = SubpassContents::Inline);

        void bindPipeline(const DeviceFunctionTable& functions,
            PipelineRef pipeline, PipelineBindPoint bindPoint);

        template<typename Container>
        void bindVertexBuffers(const DeviceFunctionTable& functions, uint32_t firstBinding,
            const Container& buffers, std::span<const DeviceSize> offsets) requires (
                std::is_same_v<std::decay_t<decltype(buffers[0])>, BufferRef> ||
                std::is_same_v<std::decay_t<decltype(buffers[0])>, Buffer>)
        {
            GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
            GRAPHICS_VERIFY(buffers.size() == offsets.size(), "Buffers and offsets sizes must match");
            functions.execute<DeviceFunction::CmdBindVertexBuffers>(getHandle(),
                firstBinding, buffers.size(), Buffer::underlyingCast(buffers.data()), offsets.data());
        }

        template<typename T>
        inline void bindIndexBuffer(const DeviceFunctionTable& functions,
            BufferRef buffer, size_t offset)
        {
            bindIndexBuffer(functions, buffer, offset, IndexTypeTraits_v<T>);
        }

        void bindIndexBuffer(const DeviceFunctionTable& functions,
            BufferRef buffer, size_t offset, IndexType indexType) 
        {
            GRAPHICS_VERIFY(isSet(), "Trying to record an invalid command buffer");
            functions.execute<DeviceFunction::CmdBindIndexBuffer>(getHandle(),
                buffer.getHandle(), offset, convertCEnum(indexType));
        }

        void bindDescriptorSets(const DeviceFunctionTable& functions, PipelineBindPoint pipelineBindPoint,
            PipelineLayoutRef pipelineLayout, uint32_t firstSet, std::span<const DescriptorSet> descriptorSets,
            std::span<const uint32_t> dynamicOffsets = {});

        void bindDescriptorSets(const DeviceFunctionTable& functions, PipelineBindPoint pipelineBindPoint,
            PipelineLayoutRef pipelineLayout, DescriptorSet descriptorSet, std::span<const uint32_t> dynamicOffsets = {});

        void setPipelineBarrier(const DeviceFunctionTable& functions, Flags::PipelineStage srcStage,
            Flags::PipelineStage dstStage, Flags::Dependency dependencyFlags,
            std::span<const MemoryBarrier> memoryBarriers,
            std::span<const BufferMemoryBarrier> bufferMemoryBarriers,
            std::span<const ImageMemoryBarrier> imageMemoryBarriers);

        void copyBuffer(const DeviceFunctionTable& functions, BufferRef srcBuffer,
            BufferRef dstBuffer, std::span<const BufferCopy> copyRegions);

        void copyBuffer(const DeviceFunctionTable& functions, BufferRef srcBuffer,
            BufferRef dstBuffer, const BufferCopy& copyRegion);

        void copyBufferToImage(const DeviceFunctionTable& functions, BufferRef srcBuffer,
            ImageRef dstImage, ImageLayout dstLayout, std::span<const BufferImageCopy> imageCopies);

        void copyBufferToImage(const DeviceFunctionTable& functions, BufferRef srcBuffer,
            ImageRef dstImage, ImageLayout dstLayout, const BufferImageCopy& imageCopy);

        void blitImage(const DeviceFunctionTable& functions,
            ImageRef srcImage, ImageRef dstImage,
            ImageLayout srcLayout, ImageLayout dstLayout,
            std::span<const ImageBlit> blit, Filter filter = Filter::Linear);

        void pushConstants(const DeviceFunctionTable& functions, PipelineLayoutRef pipelineLayout,
            Flags::ShaderStage stageFlags, uint32_t offset, uint32_t size, const void* values);

        template<typename T>
        void pushConstants(const DeviceFunctionTable& functions, PipelineLayoutRef pipelineLayout,
            Flags::ShaderStage stageFlags, const T& data) {            
            this->pushConstants(functions, pipelineLayout, stageFlags, 0, sizeof(T), &data);
        }

        void setViewports(const DeviceFunctionTable& functions, uint32_t firstViewport,
            std::span<const Viewport> viewports);
        void setScissors(const DeviceFunctionTable& functions, uint32_t firstScissor,
            std::span<const Scissor> scissors);

        void setViewport(const DeviceFunctionTable& functions, const Viewport& viewports);
        void setScissor(const DeviceFunctionTable& functions, const Scissor& scissors);

        void draw(const DeviceFunctionTable& functions,
            uint32_t vertexCount, uint32_t instanceCount,
            uint32_t firstVertex, uint32_t firstInstance);

        void drawIndexed(const DeviceFunctionTable& functions,
            uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
            uint32_t vertexOffset, uint32_t firstInstance);

        void drawIndirect(const DeviceFunctionTable& functions, BufferRef buffer,
            DeviceSize offset, uint32_t drawCount, uint32_t stride);

        void endRenderPass(const DeviceFunctionTable& functions);
        Result stopRecord(const DeviceFunctionTable& functions);
        Result reset(const DeviceFunctionTable& functions,
            Flags::CommandBufferReset flags = Flags::CommandBufferReset::Bits::None);

        void dispatch(const DeviceFunctionTable& functions,
            uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ);

        Result begin(const DeviceFunctionTable& functions, const CommandBufferBeginInfo& beginInfo);
        Result end(const DeviceFunctionTable& functions);

        void pipelineBarrier(const DeviceFunctionTable& functions,
            Flags::PipelineStage srcStage, Flags::PipelineStage dstStage,
            Flags::Dependency dependencyFlags, std::span<const MemoryBarrier> memoryBarriers,
            std::span<const BufferMemoryBarrier> bufferMemoryBarriers,
            std::span<const ImageMemoryBarrier> imageMemoryBarriers);

	};
}

