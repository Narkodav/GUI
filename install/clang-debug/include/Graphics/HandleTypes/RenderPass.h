#pragma once
#include "../Common.h"
#include "../Structs.h"
#include "../DeviceFunctionTable.h"
#include "Fence.h"
#include "Semaphore.h"

namespace Graphics
{
    class Attachment : public StructBase<VkAttachmentDescription, Attachment>
    {
		using Base = StructBase<VkAttachmentDescription, Attachment>;
    public:
        using Base::Base;

		static inline const auto s_unusedAttachment = VK_ATTACHMENT_UNUSED;        

        Attachment(Format format,
            AttachmentLoadOp loadOp = AttachmentLoadOp::DontCare,
            AttachmentStoreOp storeOp = AttachmentStoreOp::DontCare,
            AttachmentLoadOp stencilLoadOp = AttachmentLoadOp::DontCare,
            AttachmentStoreOp stencilStoreOp = AttachmentStoreOp::DontCare,
            ImageLayout initialLayout = ImageLayout::Undefined,
            ImageLayout finalLayout = ImageLayout::Undefined,
            Flags::AttachmentCreate flags = Flags::AttachmentCreate::Bits::None,
            Flags::SampleCount::Bits samples = Flags::SampleCount::Bits::SC1) : Base()
        {
            this->format = convertCEnum(format);
            this->loadOp = convertCEnum(loadOp);
            this->storeOp = convertCEnum(storeOp);
            this->stencilLoadOp = convertCEnum(stencilLoadOp);
            this->stencilStoreOp = convertCEnum(stencilStoreOp);
            this->initialLayout = convertCEnum(initialLayout);
            this->finalLayout = convertCEnum(finalLayout);
            this->flags = flags;
            this->samples = static_cast<VkSampleCountFlagBits>(samples);
        }
        Attachment& setFormat(Format format) {
            this->format = convertCEnum(format);
            return *this;
        }
        Attachment& setLoadOp(AttachmentLoadOp loadOp) {
            this->loadOp = convertCEnum(loadOp);
            return *this;
        }
        Attachment& setStoreOp(AttachmentStoreOp storeOp) {
            this->storeOp = convertCEnum(storeOp);
            return *this;
        }
        Attachment& setStencilLoadOp(AttachmentLoadOp stencilLoadOp) {
            this->stencilLoadOp = convertCEnum(stencilLoadOp);
            return *this;
        }
        Attachment& setStencilStoreOp(AttachmentStoreOp stencilStoreOp) {
            this->stencilStoreOp = convertCEnum(stencilStoreOp);
            return *this;
        }
        Attachment& setInitialLayout(ImageLayout initialLayout) {
            this->initialLayout = convertCEnum(initialLayout);
            return *this;
        }
        Attachment& setFinalLayout(ImageLayout finalLayout) {
            this->finalLayout = convertCEnum(finalLayout);
            return *this;
        }
        Attachment& setFlags(Flags::AttachmentCreate flags) {
            this->flags = flags;
            return *this;
        }
        Attachment& setSamples(Flags::SampleCount::Bits samples) {
            this->samples = static_cast<VkSampleCountFlagBits>(samples);
            return *this;
        }
	};

    class AttachmentReference : public StructBase<VkAttachmentReference, AttachmentReference>
    {
        using Base = StructBase<VkAttachmentReference, AttachmentReference>;
    public:
        using Base::Base;
        AttachmentReference(uint32_t attachmentIndex = Attachment::s_unusedAttachment,
            ImageLayout layout = ImageLayout::Undefined) : Base()
        {
            this->attachment = attachmentIndex;
            this->layout = convertCEnum(layout);
        }

        AttachmentReference& setAttachment(uint32_t attachmentIndex) {
            this->attachment = attachmentIndex;
            return *this;
        }

        AttachmentReference& setLayout(ImageLayout layout) {
            this->layout = convertCEnum(layout);
            return *this;
        }

        uint32_t getAttachment() const { return this->attachment; };
        ImageLayout getLayout() const { return convertCEnum(this->layout); };
    };    

    class Subpass : public StructBase<VkSubpassDescription, Subpass>
    {
		using Base = StructBase<VkSubpassDescription, Subpass>;
        public:
        using Base::Base;

		static inline const auto s_externalSubpass = VK_SUBPASS_EXTERNAL;

        Subpass(PipelineBindPoint bindPoint = PipelineBindPoint::Graphics,
            std::span<const AttachmentReference> inputAttachments = {},
            std::span<const AttachmentReference> colorAttachments = {},
            std::span<const AttachmentReference> resolveAttachments = {},
            const AttachmentReference* depthStencilAttachment = nullptr,
            std::span<const uint32_t> preserveAttachments = {}) : Base()
        {
            this->pipelineBindPoint = convertCEnum(bindPoint);
            this->inputAttachmentCount = inputAttachments.size();
            this->pInputAttachments = AttachmentReference::underlyingCast(inputAttachments.data());
            this->colorAttachmentCount = colorAttachments.size();
            this->pColorAttachments = AttachmentReference::underlyingCast(colorAttachments.data());
            this->pResolveAttachments = AttachmentReference::underlyingCast(resolveAttachments.data());
            this->pDepthStencilAttachment = AttachmentReference::underlyingCast(depthStencilAttachment);
            this->preserveAttachmentCount = preserveAttachments.size();
            this->pPreserveAttachments = preserveAttachments.data();
        }

        Subpass& setBindPoint(PipelineBindPoint bindPoint) {
            this->pipelineBindPoint = convertCEnum(bindPoint);
            return *this;
        }
        Subpass& setInputAttachments(std::span<const AttachmentReference> inputAttachments) {
            this->inputAttachmentCount = inputAttachments.size();
            this->pInputAttachments = AttachmentReference::underlyingCast(inputAttachments.data());
            return *this;
        }
        Subpass& setColorAttachments(std::span<const AttachmentReference> colorAttachments) {
            this->colorAttachmentCount = colorAttachments.size();
            this->pColorAttachments = AttachmentReference::underlyingCast(colorAttachments.data());
            return *this;
        }
        Subpass& setResolveAttachments(std::span<const AttachmentReference> resolveAttachments) {
            this->pResolveAttachments = AttachmentReference::underlyingCast(resolveAttachments.data());
            return *this;
        }
        Subpass& setDepthStencilAttachment(const AttachmentReference& depthStencilAttachment) {
            this->pDepthStencilAttachment = AttachmentReference::underlyingCast(&depthStencilAttachment);
            return *this;
        }
        Subpass& setPreserveAttachments(std::span<const uint32_t> preserveAttachments) {
            this->preserveAttachmentCount = preserveAttachments.size();
            this->pPreserveAttachments = preserveAttachments.data();
			return *this;
		}
	};

    class SubpassDependency : public StructBase<VkSubpassDependency, SubpassDependency>
    {
        using Base = StructBase<VkSubpassDependency, SubpassDependency>;
    public:
        using Base::Base;
        SubpassDependency(uint32_t srcSubpass = Subpass::s_externalSubpass,
            uint32_t dstSubpass = Subpass::s_externalSubpass,
            Flags::PipelineStage srcStageMask = Flags::PipelineStage::Bits::AllCommands,
            Flags::PipelineStage dstStageMask = Flags::PipelineStage::Bits::AllCommands,
            Flags::Access srcAccessMask = Flags::Access::Bits::None,
            Flags::Access dstAccessMask = Flags::Access::Bits::None,
            Flags::Dependency flags = Flags::Dependency::Bits::None) : Base()
        {
            this->srcSubpass = srcSubpass;
            this->dstSubpass = dstSubpass;
            this->srcStageMask = srcStageMask;
            this->dstStageMask = dstStageMask;
            this->srcAccessMask = srcAccessMask;
            this->dstAccessMask = dstAccessMask;
            this->dependencyFlags = flags;
        }
        SubpassDependency& setSrcSubpass(uint32_t srcSubpass) {
            this->srcSubpass = srcSubpass;
            return *this;
        }
        SubpassDependency& setDstSubpass(uint32_t dstSubpass) {
            this->dstSubpass = dstSubpass;
            return *this;
        }
        SubpassDependency& setSrcStageMask(Flags::PipelineStage srcStageMask) {
            this->srcStageMask = srcStageMask;
            return *this;
        }
        SubpassDependency& setDstStageMask(Flags::PipelineStage dstStageMask) {
            this->dstStageMask = dstStageMask;
            return *this;
        }
        SubpassDependency& setSrcAccessMask(Flags::Access srcAccessMask) {
            this->srcAccessMask = srcAccessMask;
            return *this;
        }
        SubpassDependency& setDstAccessMask(Flags::Access dstAccessMask) {
            this->dstAccessMask = dstAccessMask;
            return *this;
        }
        SubpassDependency& setDependencyFlags(Flags::Dependency flags) {
            this->dependencyFlags = flags;
            return *this;
        }
    };

    class RenderPassRef : public BaseComponent<VkRenderPass, RenderPassRef>
    {
        using Base = BaseComponent<VkRenderPass, RenderPassRef>;
    public:
        using Base::Base;
        static inline const std::string s_typeName = "RenderPass";
    };

    class RenderPassCreateInfo : public StructBase<VkRenderPassCreateInfo, RenderPassCreateInfo>
    {
        using Base = StructBase<VkRenderPassCreateInfo, RenderPassCreateInfo>;
    public:
        using Base::Base;
        RenderPassCreateInfo(std::span<const Attachment> attachments,
            std::span<const Subpass> subpasses,
            std::span<const SubpassDependency> dependencies = {}) : Base()
        {
            this->attachmentCount = attachments.size();
            this->pAttachments = Attachment::underlyingCast(attachments.data());
            this->subpassCount = subpasses.size();
            this->pSubpasses = Subpass::underlyingCast(subpasses.data());
            this->dependencyCount = dependencies.size();
            this->pDependencies = SubpassDependency
                ::underlyingCast(dependencies.data());
        }
        RenderPassCreateInfo& setAttachments(std::span<const Attachment> attachments) {
            this->attachmentCount = attachments.size();
            this->pAttachments = Attachment::underlyingCast(attachments.data());
            return *this;
        }
        RenderPassCreateInfo& setSubpasses(std::span<const Subpass> subpasses) {
            this->subpassCount = subpasses.size();
            this->pSubpasses = Subpass::underlyingCast(subpasses.data());
            return *this;
        }
        RenderPassCreateInfo& setDependencies(std::span<const SubpassDependency> dependencies) {
            this->dependencyCount = dependencies.size();
            this->pDependencies = SubpassDependency
                ::underlyingCast(dependencies.data());
            return *this;
        }
    };

    class RenderPass : public VerificatorComponent<VkRenderPass, RenderPassRef>
    {
        using Base = VerificatorComponent<VkRenderPass, RenderPassRef>;
    public:
        using Base::Base;

        void create(const DeviceFunctionTable& functions, DeviceRef device,
            const RenderPassCreateInfo& createInfo);
        void destroy(const DeviceFunctionTable& functions, DeviceRef device);
    };
}