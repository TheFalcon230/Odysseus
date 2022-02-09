#pragma once
#include "Layer.h"
#include "Odysseus//Log.h"

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

		void CreateDockingWidget(bool* open);
		void ShowSettingWidget(bool* open);

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};


}