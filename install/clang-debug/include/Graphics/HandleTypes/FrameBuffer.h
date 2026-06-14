#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Device.h"
#include "RenderPass.h"
#include "Image.h"

namespace Graphics
{
	class FrameBufferRef : public BaseComponent<VkFramebuffer, FrameBufferRef>
	{
		using Base = BaseComponent<VkFramebuffer, FrameBufferRef>;
	public:
		using Base::Base;
		static inline const std::string s_typeName = "FrameBuffer";
	};

	class FrameBufferCreateInfo : public StructBase<VkFramebufferCreateInfo, FrameBufferCreateInfo>
	{
		using Base = StructBase<VkFramebufferCreateInfo, FrameBufferCreateInfo>;
	public:
		using Base::Base;
		FrameBufferCreateInfo(RenderPassRef renderPass,
			std::span<const ImageView> attachments,
			const Extent2D& extent, uint32_t layers = 1) : Base()
		{
			this->renderPass = renderPass.getHandle();
			this->attachmentCount = attachments.size();
			this->pAttachments = ImageView::underlyingCast(attachments.data());
			this->width = extent.getWidth();
			this->height = extent.getHeight();
			this->layers = layers;
		}
		FrameBufferCreateInfo& setRenderPass(RenderPassRef renderPass) {
			this->renderPass = renderPass.getHandle();
			return *this;
		}

		template<ContiguousContainer Container>
		FrameBufferCreateInfo& setAttachments(const Container& attachments) requires
			std::same_as<ImageView, typename Container::value_type> ||
			std::same_as<ImageViewRef, typename Container::value_type>
		{
			this->attachmentCount = attachments.size();
			this->pAttachments = ImageViewRef::underlyingCast(attachments.data());
			return *this;
		}

		FrameBufferCreateInfo& setExtent(const Extent2D& extent) {
			this->width = extent.getWidth();
			this->height = extent.getHeight();
			return *this;
		}
		FrameBufferCreateInfo& setLayers(uint32_t layers) {
			this->layers = layers;
			return *this;
		}
	};

    class FrameBuffer : public VerificatorComponent<VkFramebuffer, FrameBufferRef>
    {
        using Base = VerificatorComponent<VkFramebuffer, FrameBufferRef>;
    public:
		using Base::Base;        

		void create(const DeviceFunctionTable& functions, DeviceRef device, 
			const FrameBufferCreateInfo& createInfo);
		void destroy(const DeviceFunctionTable& functions, DeviceRef device);
    };

}

