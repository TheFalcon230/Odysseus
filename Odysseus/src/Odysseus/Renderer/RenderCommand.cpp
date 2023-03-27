#include "odcpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Odysseus
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}