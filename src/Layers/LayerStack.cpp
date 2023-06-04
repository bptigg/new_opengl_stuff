#include "LayerStack.h"

LayerStack::~LayerStack()
{
	for (auto layer : m_stack)
	{
		layer->On_Detach();
	}
	m_stack.clear();
}

void LayerStack::PushLayer(std::shared_ptr<Layer> layer)
{
	m_stack.emplace(m_stack.begin() + m_index, layer);
	m_index++;
}

void LayerStack::PushOverlay(std::shared_ptr<Layer> layer)
{
	m_stack.emplace_back(layer);
}

void LayerStack::PopLayer(int index)
{
	if (index < m_stack.size())
	{
		auto it = m_stack.begin() + index;
		m_stack[index]->On_Detach();
		m_stack.erase(it);
		m_index--;
	}
}

void LayerStack::PopLayer(std::shared_ptr<Layer> layer)
{
	auto it = std::find(m_stack.begin(), m_stack.begin() + m_index, layer);
	int index = it - m_stack.begin();
	PopLayer(index);
}


