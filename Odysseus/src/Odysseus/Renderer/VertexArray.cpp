#include "odcpch.h"
#include "VertexArray.h"

#include "Renderer.h" 
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Odysseus
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ODC_CORE_ASSERT(false, "API: None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
			break;
		default:
			ODC_CORE_ASSERT(false, "Unknown RendererAPI .");
			return nullptr;
			break;
		}
	}
}

