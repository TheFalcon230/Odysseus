#include "odcpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Odysseus {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		ODC_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ODC_CORE_ASSERT(status, "Failed to initialize Glad!");

		ODC_CORE_INFO("OpenGL Info:");
		ODC_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		ODC_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		ODC_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}