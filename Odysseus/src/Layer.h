#pragma once

#include "Odysseus/Core.h"
#include "Odysseus/Events/Event.h"

namespace Odysseus
{
	class ODYSSEUS_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& event) {};

		inline const std::string& GetName() const { return m_debugName; };

	protected:
		std::string m_debugName;
	};

}