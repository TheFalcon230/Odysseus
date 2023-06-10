#pragma once
#include "Odysseus/Core/Core.h"


namespace Odysseus
{
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class ODYSSEUS_API Framebuffer
	{
	public:
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	private:

	};
}