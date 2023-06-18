#pragma once

#include "Odysseus/Renderer/Framebuffer.h"

namespace Odysseus
{
	class  OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { ODC_CORE_ASSERT(index < ColorAttachments.size(), "") return ColorAttachments[index]; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		virtual void Resize(uint32_t width, uint32_t height) override;
	private:
		uint32_t m_RendererID = 0;
		//uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> colorAttachmentSpecs;
		FramebufferTextureSpecification depthAttachmentSpecs = FramebufferTextureFormat::None;

		std::vector<uint32_t> ColorAttachments;

		// Inherited via Framebuffer
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		// Inherited via Framebuffer
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;
	};
}