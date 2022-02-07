#pragma once
#include "Layer.h"
#include "Odysseus/Events/ApplicationEvent.h"
#include <Odysseus/Events/MouseEvent.h>
#include <Odysseus/Events/KeyEvent.h>

namespace Odysseus
{
	class ODYSSEUS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		//MOUSE EVENTS
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);

		//KEYBOARD EVENTS
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleaseEvent& event);
		bool OnKeyTyped(KeyTypedEvent& event);

		//WINDOW EVENTS
		bool OnWindowResized(WindowResizeEvent& event);
	private:
		float m_Time = 0.0f;
	};


}