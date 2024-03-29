#pragma once
#include "Odysseus/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Odysseus
{
	class ODYSSEUS_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); };
		std::vector<Layer*>::iterator end() { return m_Layers.end(); };

	private:
		std::vector<Layer*> m_Layers;
		unsigned int iLayerInsertIndex = 0;
	};

}
