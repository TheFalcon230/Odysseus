#pragma once


#include "Odysseus/Core/Window.h"
#include "Odysseus/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace Odysseus
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void Update() override;

		inline uint32_t GetWidth() const override { return m_Data.width; }
		inline uint32_t GetHeight() const override { return m_Data.height; }

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
		GraphicsContext* m_context;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

		// Hérité via Window
		virtual void* GetNativeWindow() const override;
	};
}