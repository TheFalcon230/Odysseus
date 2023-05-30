#pragma once
#include "Odysseus/Core/Layer.h"

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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};


}