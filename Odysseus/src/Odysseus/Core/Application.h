#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Odysseus/Events/Event.h"
#include "Odysseus/Events/ApplicationEvent.h"

#include "Odysseus/Core/Timestep.h"

#include "Odysseus/ImGui/ImGuiLayer.h"


namespace Odysseus
{
	class ODYSSEUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		//Layers 

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* Overlay);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
		inline void QuitApplication() { m_Running = false; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		/// <summary>
		/// Is the application is running ?
		/// </summary>
		bool m_Running = true;
		/// <summary>
		/// Is the application minimized ?
		/// </summary>
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_lastFrameTime = 0.f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
