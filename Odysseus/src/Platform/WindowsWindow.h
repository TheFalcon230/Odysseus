#pragma once


#include "Odysseus/Window.h"

#include <glfw3.h>

namespace Odysseus
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void Update() override;

		inline unsigned int GetWidth() const override { return m_Data.width; }
		inline unsigned int GetHeight() const override { return m_Data.height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		// Privates functions
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		// Privates variables
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}