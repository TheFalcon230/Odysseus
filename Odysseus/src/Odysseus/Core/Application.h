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
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			ODC_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	class  Application
	{
	public:
		ODYSSEUS_API Application(const std::string& appName = "Odysseus Editor", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		ODYSSEUS_API virtual ~Application();

		ODYSSEUS_API void Run();

		ODYSSEUS_API void OnEvent(Event& e);

		//Layers 

		ODYSSEUS_API void PushLayer(Layer* layer);
		ODYSSEUS_API void PushOverlay(Layer* Overlay);

		ODYSSEUS_API inline Window& GetWindow() { return *m_Window; }

		ODYSSEUS_API inline static Application& Get() { return *s_Instance; }
		ODYSSEUS_API ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

		ODYSSEUS_API inline void QuitApplication() { m_Running = false; }

		ODYSSEUS_API ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		ApplicationCommandLineArgs m_CommandLineArgs;
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
	Application* CreateApplication(ApplicationCommandLineArgs args);
}
