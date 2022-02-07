#include "odcpch.h"
#include "WindowsInput.h"

#include <glfw3.h>
#include "Odysseus/Application.h"

namespace Odysseus
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImp(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonPressedImp(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}
	float WindowsInput::GetMouseXImp()
	{
		auto [x, y] = GetMousePosImp();
		return x;
	}
	float WindowsInput::GetMouseYImp()
	{
		auto[x, y] =GetMousePosImp();
		return y;
	}
	std::pair<float, float> WindowsInput::GetMousePosImp()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return std::pair<float, float>(static_cast<float>(x), static_cast<float>(y));
	}
}