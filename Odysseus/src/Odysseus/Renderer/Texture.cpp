#include "odcpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Odysseus
{
	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ODC_CORE_ASSERT(false, "API: None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(specification);
			break;
		default:
			ODC_CORE_ASSERT(false, "Unknown RendererAPI .");
			return nullptr;
			break;
		}
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ODC_CORE_ASSERT(false, "API: None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
			break;
		default:
			ODC_CORE_ASSERT(false, "Unknown RendererAPI .");
			return nullptr;
			break;
		}
	}
}