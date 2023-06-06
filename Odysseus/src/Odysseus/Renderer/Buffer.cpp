#include "odcpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include"Platform/OpenGL/OpenGLBuffer.h"

namespace Odysseus
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ODC_CORE_ASSERT(false, "API: None is currently not supported.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		}

		ODC_CORE_ASSERT(false, "Unknown RendererAPI .");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ODC_CORE_ASSERT(false, "API: None is currently not supported.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		ODC_CORE_ASSERT(false, "Unknown RendererAPI .");
		return nullptr;


	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ODC_CORE_ASSERT(false, "API: None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);
			break;
		default:
			ODC_CORE_ASSERT(false, "Unknown RendererAPI .");
			return nullptr;
			break;
		}
	}
}