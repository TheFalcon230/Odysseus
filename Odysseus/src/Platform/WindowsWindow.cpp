#include "odcpch.h"
#include "WindowsWindow.h"
#include "Odysseus/Log.h"

namespace Odysseus
{
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{

		glfwSwapInterval((int)enabled);

		m_Data.vSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.vSync;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.title = props.Title;
		m_Data.width = props.width;
		m_Data.height = props.height;

		ODC_CORE_INFO("Creating window {0} {1}, {2}", props.Title, props.width, props.height);

		if (!s_GLFWInitialized)
		{
			//TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			ODC_CORE_ASSERT(success, "Could not initialize GLFW");

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

}