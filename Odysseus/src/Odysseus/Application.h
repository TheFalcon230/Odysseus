#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Odysseus/Events/Event.h"
#include "Odysseus/Events/ApplicationEvent.h"

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
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
