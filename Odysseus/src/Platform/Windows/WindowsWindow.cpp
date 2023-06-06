#include "odcpch.h"
#include "WindowsWindow.h"


//Including Events
#include "Odysseus/Events/ApplicationEvent.h"
#include "Odysseus/Events/KeyEvent.h"
#include "Odysseus/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"


namespace Odysseus
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallack(int error, const char* description)
	{
		ODC_CORE_ERROR("GLFW error {0}: {1}", error, description);
	}

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
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

		glfwSwapInterval(enabled ? 1 : 0);

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
			glfwSetErrorCallback(GLFWErrorCallack);
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
		m_context = new OpenGLContext(m_Window);
		m_context->Init();
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		//Set GLFW callbacks

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(window));
		data.width = width;
		data.height = height;

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(window));

		WindowCloseEvent event;
		data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent event(key, 0);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleaseEvent event(key);
			data.EventCallback(event);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent event(key, 1);
			data.EventCallback(event);
			break;
		}
		default:
			break;
		}
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(button);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(button);
			data.EventCallback(event);
			break;
		}
		default:
			break;
		}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(window));

		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double posX, double posY)
			{
				WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(window));

		MouseMovedEvent event(static_cast<float>(posX), static_cast<float>(posY));
		data.EventCallback(event);
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keyCode)
			{
				WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(window));
		KeyTypedEvent event(keyCode);
		data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}

}