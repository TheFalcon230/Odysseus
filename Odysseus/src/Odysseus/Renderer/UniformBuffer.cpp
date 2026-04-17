#include "odcpch.h"
#include "UniformBuffer.h"

#include "Odysseus/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Odysseus
{
	Ref<UniformBuffer> Odysseus::UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: ODC_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLUniformBuffer>(size, binding);
		default:
			break;
		}

		ODC_CORE_ASSERT(false, "Unkown RendererAPI !");
		return nullptr;
	}

}