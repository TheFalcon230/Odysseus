#include "odcpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Odysseus
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ODC_CORE_ASSERT(false, "API: None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
			break;
		default:
			ODC_CORE_ASSERT(false, "Unknown RendererAPI .");
			return nullptr;
			break;
		}
	}
}