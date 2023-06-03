#pragma once

#include "Layer.h"

#include <vector>
#include <memory>


class LayerStack
{
public:
	LayerStack() = default;
	~LayerStack();

	void PushLayer(std::shared_ptr<Layer> layer);
	void PushOverlay(std::shared_ptr<Layer> layer);
	void PopLayer(int index);
	void PopLayer(std::shared_ptr<Layer> layer);
	
	inline int size() { return m_stack.size(); }
	inline std::shared_ptr<Layer> back() { return m_stack.back(); }

	std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_stack.begin(); }
	std::vector<std::shared_ptr<Layer>>::iterator end() { return m_stack.end(); }
	std::vector<std::shared_ptr<Layer>>::reverse_iterator r_begin() { return m_stack.rbegin(); }
	std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return m_stack.rend(); }

	std::vector<std::shared_ptr<Layer>>::const_iterator begin() const { return m_stack.begin(); }
	std::vector<std::shared_ptr<Layer>>::const_iterator end() const { return m_stack.end(); }
	std::vector<std::shared_ptr<Layer>>::const_reverse_iterator r_begin() const { return m_stack.rbegin(); }
	std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rend() const { return m_stack.rend(); }

	inline std::shared_ptr<Layer> operator[](unsigned int index)
	{
		return m_stack[index];
	}

private:
	std::vector<std::shared_ptr<Layer>> m_stack;
	unsigned int m_index = 0;

};

