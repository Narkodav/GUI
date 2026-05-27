#pragma once
#include "GUI/Instance.h"
#include "GUI/Elements/QuadElement.h"

#include "PlatformKit/Window.h"
#include "Graphics/Graphics.h"

class Test {

private:
    Graphics::Wrappers::Instance m_instance;
    Graphics::Wrappers::Device m_device;

    Graphics::Surface m_surface;
    PlatformKit::Window m_window;

    Graphics::PhysicalDevice m_physicalDevice;

    Graphics::Queue m_graphicsQueue;
    Graphics::Queue m_presentQueue;
    Graphics::Queue m_transferQueue;

    Graphics::RenderRegion m_canvas;

    Graphics::Utility::RenderPassData m_renderPassData;
    Graphics::Utility::SwapChainData m_swapChainData;

    Graphics::CommandPool m_graphicsCommandPool;
    Graphics::CommandPool m_temporaryBufferPool;

    Graphics::DescriptorPool m_descriptorPool;


    Graphics::CommandBuffer m_graphicsCommandBuffer;
    Graphics::Semaphore m_imageAvailableSemaphore;
    std::vector<Graphics::Semaphore> m_renderFinishedSemaphore;
    Graphics::Fence m_inFlightFence;

    GUI::Instance m_guiInstance;

    std::unique_ptr<GUI::QuadElement> m_button;


public:

    void create() {
        m_window.create({800, 800}, "test");
        m_instance.create({"test", "test", {1, 0, 0}, {1, 0, 0}});
        m_instance.cachePhysicalDevices();
        m_surface.create(m_instance.getFunctionTable(), m_instance, m_window);

        Graphics::DeviceRequirements requirements;
        requirements.extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME 
        };
        requirements.properties = { 
            {Graphics::DeviceProperty::PhysicalDeviceType, Graphics::PhysicalDeviceType::DiscreteGpu} };
        requirements.features = {
            {Graphics::DeviceFeature::GeometryShader, true},
            {Graphics::DeviceFeature::SamplerAnisotropy, true},
            {Graphics::DeviceFeature::DescriptorBindingPartiallyBound, true},
            {Graphics::DeviceFeature::RuntimeDescriptorArray, true},
            {Graphics::DeviceFeature::ShaderSampledImageArrayNonUniformIndexing, true},
            {Graphics::DeviceFeature::DescriptorBindingSampledImageUpdateAfterBind, true},
            {Graphics::DeviceFeature::DescriptorBindingVariableDescriptorCount, true},
            {Graphics::DeviceFeature::MeshShader, true},
            {Graphics::DeviceFeature::TaskShader, true},
            {Graphics::DeviceFeature::ShaderDrawParameters, true},
            {Graphics::DeviceFeature::MultiDrawIndirect, true},
            {Graphics::DeviceFeature::DescriptorBindingStorageBufferUpdateAfterBind, true},
            {Graphics::DeviceFeature::NullDescriptor, true}, 
            {Graphics::DeviceFeature::DescriptorIndexing, true}, 
        };

        requirements.queueProperties.push_back(Graphics::RequiredQueueProperties());
        requirements.queueProperties.back().queueProperties
            .insert({ Graphics::QueueProperty::QueueFlags, Graphics::Flags::Queue::Bits::Graphics | Graphics::Flags::Queue::Bits::Transfer });
        requirements.queueProperties.back().shouldSupportPresent = false;

        requirements.queueProperties.push_back(Graphics::RequiredQueueProperties());
        requirements.queueProperties.back().shouldSupportPresent = true;

        auto result = m_instance.getFittingDevice(m_surface, requirements);
        if (!result.isSuitable())
            std::cout << "No suitable device found" << std::endl;
        else std::cout << "Suitable device found" << std::endl;
        m_physicalDevice = result.device;

        //search for discrete graphics, present and transfer families
        uint32_t graphicsIndex, presentIndex;

        graphicsIndex = result.queueFamilyIndices[0].front();
        size_t i;

        for (i = 0; i < result.queueFamilyIndices[1].size(); i++)
            if (graphicsIndex != result.queueFamilyIndices[1][i])
            {
                presentIndex = result.queueFamilyIndices[1][i];
                break;
            }
        if (i == result.queueFamilyIndices[1].size()) {
            presentIndex = result.queueFamilyIndices[1].front();
        }
        
        std::vector<std::vector<float>> queuePriorities = { {1.0f, 1.0f}, {1.0f} };
        std::vector<Graphics::QueueCreateInfo> queueCreateInfos(2);
        queueCreateInfos[0].setQueueFamilyIndex(graphicsIndex)
            .setQueuePriorities(queuePriorities[0]);
        queueCreateInfos[1].setQueueFamilyIndex(presentIndex)
            .setQueuePriorities(queuePriorities[1]);

        Graphics::DeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.setQueueCreateInfos(queueCreateInfos)
            .setEnabledExtensions(requirements.extensions)
            .setEnabledFeatures(result.enabledFeatures);
            
        m_device.create(m_instance, m_physicalDevice, deviceCreateInfo);
        
        m_graphicsQueue = m_device.getQueue(graphicsIndex, 0);
        m_presentQueue = m_device.getQueue(presentIndex, 0);
        m_transferQueue = m_device.getQueue(graphicsIndex, 1);
        
        m_canvas = Graphics::RenderRegion::createFullWindow(m_window.getWindowExtent());
        
        auto capabilities = m_physicalDevice.getSurfaceCapabilities(m_instance.getFunctionTable(), m_surface);
        auto formats = m_physicalDevice.getSurfaceFormats(m_instance.getFunctionTable(), m_surface);
        auto presentModes = m_physicalDevice.getSurfacePresentModes(m_instance.getFunctionTable(), m_surface);

        Graphics::Format depthFormat = Graphics::Utility::findDepthFormat(m_instance.getFunctionTable(), m_physicalDevice);

        if (depthFormat == Graphics::Format::Undefined) {
            throw std::runtime_error("Failed to find supported depth format");
        }

        m_renderPassData = Graphics::Utility::createColorDepthRenderPass(m_device.getFunctionTable(),
            m_device, formats, presentModes, depthFormat);

        m_swapChainData = Graphics::Utility::createBasicSwapChain(m_instance.getFunctionTable(),
            m_device.getFunctionTable(), m_physicalDevice, m_device, m_surface, m_renderPassData.renderPass,
            Graphics::Utility::chooseExtent(capabilities, m_window.getFrameBufferExtent()),
            m_renderPassData.surfaceFormat.getFormat(), depthFormat, m_renderPassData.surfaceFormat.getColorSpace(),
            Graphics::Flags::ImageUsage::Bits::ColorAttachment, m_renderPassData.presentMode, capabilities.getMinImageCount());

        m_renderFinishedSemaphore.resize(m_swapChainData.swapChainImages.size());
        for(size_t i = 0; i < m_renderFinishedSemaphore.size(); ++i) 
            m_renderFinishedSemaphore[i].create(m_device.getFunctionTable(), m_device);

        std::vector<Graphics::DescriptorPoolSize> poolSizes = {
            Graphics::DescriptorPoolSize(3, Graphics::DescriptorType::UniformBuffer),
            Graphics::DescriptorPoolSize(11 + 3 + 1024, Graphics::DescriptorType::StorageBuffer),
            Graphics::DescriptorPoolSize(1024, Graphics::DescriptorType::CombinedImageSampler)
        };

        m_descriptorPool.create(m_device.getFunctionTable(), m_device, 
            Graphics::DescriptorPoolCreateInfo(4, poolSizes, Graphics::Flags::DescriptorPoolCreate::Bits::UpdateAfterBind));

        m_graphicsCommandPool.create(m_device.getFunctionTable(), m_device,
            Graphics::CommandPoolCreateInfo(graphicsIndex));

        m_temporaryBufferPool.create(m_device.getFunctionTable(), m_device,
            Graphics::CommandPoolCreateInfo(graphicsIndex));

        m_graphicsCommandBuffer = m_graphicsCommandPool.allocateCommandBuffer(m_device.getFunctionTable(), m_device);
        m_imageAvailableSemaphore.create(m_device.getFunctionTable(), m_device);
        m_inFlightFence.create(m_device.getFunctionTable(), m_device,
            { Graphics::Flags::FenceCreate::Bits::Signaled });

        m_guiInstance.create(m_instance.getFunctionTable(), m_device.getFunctionTable(), m_device, 
            m_physicalDevice, m_descriptorPool, m_renderPassData.renderPass);

        m_button = std::make_unique<GUI::QuadElement>();
        m_button->getQuad().setPosition(glm::ivec2(100, 100));
        m_button->getQuad().setSize(glm::ivec2(100, 100));

        m_window.registerCallback<PlatformKit::IOEvents::MouseMovedScreen>([this](PlatformKit::Position mousePosition){
            GUI::PointerEvent event = {
                GUI::PointerEvent::Type::Move,
                {
                    glm::ivec2(mousePosition.x, mousePosition.y),
                    glm::vec2(0, 0)
                }
            };
            std::cout << event.pointer.position.x << " " << event.pointer.position.y << std::endl;
            m_button->pointerEvent(event);
        });

        m_button->setCallback<GUI::QuadElementEvent::MovedIn>(
            [](const GUI::Pointer& pointer, GUI::QuadElement& button){
                button.getQuad().setTexture(static_cast<GUI::TextureId>(GUI::DefaultTextureCache::TextureType::BlackTexture));
            }
        );
        m_button->setCallback<GUI::QuadElementEvent::MovedOut>(
            [](const GUI::Pointer& pointer, GUI::QuadElement& button){
                button.getQuad().setTexture(static_cast<GUI::TextureId>(GUI::DefaultTextureCache::TextureType::WhiteTexture));
            }
        );
    }

    void destroy()
    {
        m_device.waitIdle();
        m_guiInstance.destroy(m_device.getFunctionTable(), m_device);
        
        m_temporaryBufferPool.destroy(m_device.getFunctionTable(), m_device);

        m_imageAvailableSemaphore.destroy(m_device.getFunctionTable(), m_device);
        for(size_t i = 0; i < m_renderFinishedSemaphore.size(); ++i) 
            m_renderFinishedSemaphore[i].destroy(m_device.getFunctionTable(), m_device);
        m_inFlightFence.destroy(m_device.getFunctionTable(), m_device);

        m_graphicsCommandPool.reset(m_device.getFunctionTable(), m_device);
        m_graphicsCommandPool.destroy(m_device.getFunctionTable(), m_device);

        m_descriptorPool.destroy(m_device.getFunctionTable(), m_device);
        m_renderPassData.renderPass.destroy(m_device.getFunctionTable(), m_device);

        Graphics::Utility::destroySwapChainData(m_device.getFunctionTable(), m_device, m_swapChainData);
        
        m_surface.destroy(m_instance.getFunctionTable(), m_instance);
        m_device.destroy();
        m_instance.destroy();
    }

    void handleResize() {
        m_device.waitIdle();
        auto surfaceCapabilities = m_physicalDevice.getSurfaceCapabilities(m_instance.getFunctionTable(), m_surface);
        m_canvas = Graphics::RenderRegion::createFullWindow(surfaceCapabilities.getCurrentExtent());

        auto extent = m_window.getFrameBufferExtent();
        Graphics::Utility::recreateBasicSwapChain(m_swapChainData, m_device.getFunctionTable(),
            m_device, m_renderPassData.renderPass, m_physicalDevice.getMemoryProperties(m_instance.getFunctionTable()), 
            Graphics::Utility::chooseExtent(surfaceCapabilities, extent));

        for(size_t i = 0; i < m_renderFinishedSemaphore.size(); ++i) 
            m_renderFinishedSemaphore[i].destroy(m_device.getFunctionTable(), m_device);

        m_renderFinishedSemaphore.resize(m_swapChainData.swapChainImages.size());

        for(size_t i = 0; i < m_renderFinishedSemaphore.size(); ++i) 
            m_renderFinishedSemaphore[i].create(m_device.getFunctionTable(), m_device);
    }
    
    void start() {
        while(!m_window.shouldClose()) {
            m_window.pollEvents();
            drawFrame(m_window.getFrameBufferExtent());
        }
    }

    void drawFrame(const Graphics::Extent2D& extent) {
        m_inFlightFence.wait(m_device.getFunctionTable(), m_device);
        m_inFlightFence.reset(m_device.getFunctionTable(), m_device);

        uint32_t imageIndex;
        auto imageAquireResult = m_swapChainData.swapChain.acquireNextImage(m_device.getFunctionTable(), 
            m_device, m_imageAvailableSemaphore, imageIndex);

        if (imageAquireResult == Graphics::Result::ErrorOutOfDateKHR ||
            imageAquireResult == Graphics::Result::SuboptimalKHR) {
                handleResize();
                return;
        }

        m_graphicsCommandBuffer.reset(m_device.getFunctionTable());
        
        m_graphicsCommandBuffer.begin(m_device.getFunctionTable(), Graphics::CommandBufferBeginInfo());

        std::array<Graphics::ClearValue, 2> clearValues = {
            Graphics::ClearColorValue(Graphics::Color::green()),
            Graphics::ClearDepthStencilValue{1.0f, 0}
        };

        Graphics::RenderPassBeginInfo renderPassBeginInfo = { m_renderPassData.renderPass, 
            m_swapChainData.swapChainFrameBuffers[imageIndex], clearValues, {0, 0}, 
            m_swapChainData.swapChainInfo.getImageExtent() };

        m_graphicsCommandBuffer.beginRenderPass(m_device.getFunctionTable(),
            renderPassBeginInfo, Graphics::SubpassContents::Inline);

        m_graphicsCommandBuffer.setViewport(m_device.getFunctionTable(), m_canvas.getViewport());
        m_graphicsCommandBuffer.setScissor(m_device.getFunctionTable(), m_canvas.getScissor());

        m_guiInstance.record(m_device.getFunctionTable(), m_device, m_graphicsCommandBuffer, m_button.get(), extent);

        m_graphicsCommandBuffer.endRenderPass(m_device.getFunctionTable());
        m_graphicsCommandBuffer.stopRecord(m_device.getFunctionTable());

        try
        {
            std::array<Graphics::Flags::PipelineStage, 1> pipelineStage = { Graphics::Flags::PipelineStage::Bits::ColorAttachmentOutput };

            Graphics::QueueSubmitInfo submitInfo(
                std::span(&m_graphicsCommandBuffer, 1),
                pipelineStage,
                std::span(&m_imageAvailableSemaphore, 1),
                std::span(&m_renderFinishedSemaphore[imageIndex], 1));

            m_graphicsQueue.submit(m_device.getFunctionTable(), submitInfo,
                m_inFlightFence);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to submit graphics queue: " << e.what() << std::endl;
        }

        Graphics::QueuePresentInfo presentInfo = {
            std::span(&m_renderFinishedSemaphore[imageIndex], 1),
            std::span(&m_swapChainData.swapChain, 1),
            std::span(&imageIndex, 1),
        };
        auto presentResult = m_presentQueue.present(m_device.getFunctionTable(), presentInfo);
        if (presentResult == Graphics::Result::ErrorOutOfDateKHR ||
            presentResult == Graphics::Result::SuboptimalKHR) {
                handleResize();
                return;
        }
    }

};