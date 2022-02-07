#include "odcpch.h"
#include "LayerStack.h"

Odysseus::LayerStack::LayerStack()
{
	m_LayerInsert = m_Layers.begin();
}

Odysseus::LayerStack::~LayerStack()
{
	for (auto layer : m_Layers)
	{
		delete layer;
	}
}

void Odysseus::LayerStack::PushLayer(Layer* layer)
{
	m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
}

void Odysseus::LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void Odysseus::LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
		m_LayerInsert--;
	}
}

void Odysseus::LayerStack::PopOverlay(Layer* overlay)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it!= m_Layers.end())
	{
		m_Layers.erase(it);
	}
}
