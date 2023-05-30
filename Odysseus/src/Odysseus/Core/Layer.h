#pragma once

#include "Odysseus/Core/Core.h"
#include "Odysseus/Events/Event.h"
#include "Odysseus/Core/Timestep.h"

namespace Odysseus
{
	class ODYSSEUS_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(Timestep updateTime) {};
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {};

		inline const std::string& GetName() const { return m_debugName; };

	protected:
		std::string m_debugName;
	};

}
