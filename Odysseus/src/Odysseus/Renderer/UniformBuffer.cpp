#include "odcpch.h"
#include "UniformBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Odysseus
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding, BufferUsageType usage)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
			//ODYSSEUS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLUniformBuffer>(size, binding, usage);
		default:
			break;
		}

		return Ref<UniformBuffer>();
	}

}