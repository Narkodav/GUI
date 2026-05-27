#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Fence.h"
#include "Semaphore.h"
#include "CommandBuffer.h"
#include "SwapChain.h"

namespace Graphics
{
    class QueueSubmitInfo : public StructBase<VkSubmitInfo, QueueSubmitInfo>
    {
        using Base = StructBase<VkSubmitInfo, QueueSubmitInfo>;
    public:
        using Base::Base;

        template<ContiguousContainer ContainerWait, ContiguousContainer ContainerSignal>
        QueueSubmitInfo(std::span<const CommandBuffer> commandBuffers,
            std::span<const Flags::PipelineStage> waitStages = {},
            const ContainerWait& waitSemaphores = {},
            const ContainerSignal& signalSemaphores = {}) requires
            (std::is_convertible_v<typename ContainerWait::value_type, Semaphore> ||
                std::is_convertible_v<typename ContainerWait::value_type, SemaphoreRef>) &&
            (std::is_convertible_v<typename ContainerSignal::value_type, Semaphore> ||
                std::is_convertible_v<typename ContainerSignal::value_type, SemaphoreRef>) : Base() {
            this->commandBufferCount = commandBuffers.size();
            this->pCommandBuffers = CommandBuffer::underlyingCast(commandBuffers.data());
            this->signalSemaphoreCount = signalSemaphores.size();
            this->pSignalSemaphores = Semaphore::underlyingCast(signalSemaphores.data());
            this->waitSemaphoreCount = waitSemaphores.size();
            this->pWaitSemaphores = Semaphore::underlyingCast(waitSemaphores.data());
            this->pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>(waitStages.data());
        }

        template<ContiguousContainer ContainerWait>
        QueueSubmitInfo& setWaitStages(std::span<const Flags::PipelineStage> waitStages, const ContainerWait& waitSemaphores) requires
            std::is_convertible_v<typename ContainerWait::value_type, Semaphore> ||
            std::is_convertible_v<typename ContainerWait::value_type, SemaphoreRef> {
            GRAPHICS_VERIFY(waitStages.size() == waitSemaphores.size(), "Wait stages and wait semaphores must have the same size");
            this->waitSemaphoreCount = waitSemaphores.size();
            this->pWaitSemaphores = Semaphore::underlyingCast(waitSemaphores.data());
            this->pWaitDstStageMask = reinterpret_cast<const VkPipelineStageFlags*>(waitStages.data());
            return *this;
        }

        template<ContiguousContainer ContainerSignal>
        QueueSubmitInfo& setSignalSemaphores(const ContainerSignal& signalSemaphores) requires
            std::is_convertible_v<typename ContainerSignal::value_type, Semaphore> ||
            std::is_convertible_v<typename ContainerSignal::value_type, SemaphoreRef> {
            this->signalSemaphoreCount = signalSemaphores.size();
            this->pSignalSemaphores = Semaphore::underlyingCast(signalSemaphores.data());
            return *this;
        }

        QueueSubmitInfo& setCommandBuffers(std::span<const CommandBuffer> commandBuffers) {
            this->commandBufferCount = commandBuffers.size();
            this->pCommandBuffers = CommandBuffer::underlyingCast(commandBuffers.data());
            return *this;
        }

        QueueSubmitInfo& setCommandBuffer(const CommandBuffer& commandBuffers) {
            this->commandBufferCount = 1;
            this->pCommandBuffers = CommandBuffer::underlyingCast(&commandBuffers);
            return *this;
        }
    };

    class QueuePresentInfo : public StructBase<VkPresentInfoKHR, QueuePresentInfo>
    {
        using Base = StructBase<VkPresentInfoKHR, QueuePresentInfo>;
    public:
        using Base::Base;

        template<ContiguousContainer ContainerWait, ContiguousContainer ContainerSwapChain>
        QueuePresentInfo(const ContainerWait& waitSemaphores,
            const ContainerSwapChain& swapChains,
            std::span<const uint32_t> imageIndices) requires
            (std::is_convertible_v<typename ContainerWait::value_type, Semaphore> ||
                std::is_convertible_v<typename ContainerWait::value_type, SemaphoreRef>) &&
            (std::is_convertible_v<typename ContainerSwapChain::value_type, SwapChain> ||
                std::is_convertible_v<typename ContainerSwapChain::value_type, SwapChainRef>) : Base() {
            this->swapchainCount = swapChains.size();
            this->pSwapchains = SwapChain::underlyingCast(swapChains.data());
            this->pImageIndices = imageIndices.data();
            this->waitSemaphoreCount = waitSemaphores.size();
            this->pWaitSemaphores = Semaphore::underlyingCast(waitSemaphores.data());
        }

        template<ContiguousContainer ContainerWait>
        QueuePresentInfo& setWaitSemaphores(const ContainerWait& waitSemaphores) requires
            std::is_convertible_v<typename ContainerWait::value_type, Semaphore> ||
            std::is_convertible_v<typename ContainerWait::value_type, SemaphoreRef> {
            this->waitSemaphoreCount = waitSemaphores.size();
            this->pWaitSemaphores = Semaphore::underlyingCast(waitSemaphores.data());
            return *this;
        }

        template<ContiguousContainer ContainerSwapChain>
        QueuePresentInfo& setSwapChains(const ContainerSwapChain& swapChains,
            std::span<const uint32_t> imageIndices) requires
            std::is_convertible_v<typename ContainerSwapChain::value_type, SwapChain> ||
            std::is_convertible_v<typename ContainerSwapChain::value_type, SwapChainRef> {
            GRAPHICS_VERIFY(swapChains.size() == imageIndices.size(), "swap chains and image indices must have the same size");
            this->swapchainCount = swapChains.size();
            this->pSwapchains = SwapChain::underlyingCast(swapChains.data());
            this->pImageIndices = imageIndices.data();
            return *this;
        }
    };

    class Queue : public BaseComponent<VkQueue, Queue>
    {
        friend class DeviceRef;
        friend class Device;
        using Base = BaseComponent<VkQueue, Queue>;
    public:
        using Base::Base;        

        Result waitIdle(const DeviceFunctionTable& functions) const;

        Result submit(const DeviceFunctionTable& functions,
            std::span<const QueueSubmitInfo> submitInfos,
            const FenceRef& fence) const;

        Result submit(const DeviceFunctionTable& functions,
            const QueueSubmitInfo& submitInfo,
            const FenceRef& fence) const;

        Result present(const DeviceFunctionTable& functions,
            const QueuePresentInfo& presentInfo) const;
    };
}