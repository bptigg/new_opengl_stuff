#include "Rendering_manager.h"

#include <vector>

#include "../utilities/utility.h"

struct manager_data
{
	render_queue queue;
	
	bool sorted = false;
	int num_layers = 0;
	int num_textures = 0;

	int active_layer_index = 0;
	int active_texture_index = 0;

	bool finished = false;
};

static manager_data* s_data = nullptr;

void Rendering_manager::Init()
{
	s_data = new manager_data;
}

void Rendering_manager::Shutdown()
{
	s_data->queue.reset_queue();
	delete s_data;
}

void Rendering_manager::submit_queue_object(std::shared_ptr<render_object>& object)
{
	//s_data->m_objects.push_back(object);
	s_data->queue.push(object);
	s_data->finished = false;
}

std::vector<std::shared_ptr<render_object>> Rendering_manager::flush()
{
	if (s_data->sorted == false)
	{
		s_data->num_layers = s_data->queue.m_active_layers.size();
		Utility::quicksort<uint32_t>(s_data->queue.m_active_layers, 0, s_data->num_layers - 1);
		s_data->queue.m_min_max[0] = s_data->queue.m_active_layers[0];
		s_data->queue.m_min_max[1] = s_data->queue.m_active_layers[s_data->num_layers - 1];
		s_data->sorted = true;

		s_data->active_layer_index = 0;
		s_data->active_texture_index = 0;
	}

	if (s_data->active_layer_index == s_data->queue.m_active_layers.size())
	{
		s_data->finished = true;
		//s_data->queue.reset_queue();
		return {};
	}

	auto objects = s_data->queue.m_queue[s_data->queue.m_active_layers[s_data->active_layer_index]];
	auto specfic_texture = objects[s_data->queue.m_active_textures[s_data->active_texture_index]];

	for (int i = s_data->active_texture_index + 1; i < s_data->queue.m_active_textures.size(); i++)
	{
		if (objects.find(s_data->queue.m_active_textures[i]) == objects.end())
		{
			continue;
		}
		else
		{
			break;
		}
	}

	s_data->active_texture_index++;

	if (s_data->active_texture_index >= objects.size())
	{
		s_data->active_layer_index++;
		s_data->active_texture_index = 0;
	}

	return specfic_texture;
}

void render_queue::push(std::shared_ptr<render_object>& object)
{
	if (m_queue.find(object->layer) == m_queue.end())
	{
		m_active_layers.push_back(object->layer);
	}

	if (std::find(m_active_textures.begin(), m_active_textures.end(), object->Texture) == m_active_textures.end())
	{
		m_active_textures.push_back(object->Texture);
	}

	m_queue[object->layer][object->Texture].push_back(object);
}

void render_queue::reset_queue()
{
	for (auto it = m_queue.begin(); it != m_queue.end(); it++)
	{
		for (auto it_2 = it->second.begin(); it_2 != it->second.end(); it_2++)
		{
			it_2->second.clear();
		}
		it->second.clear();
	}
	m_queue.clear();

	m_active_layers.clear();
	m_active_textures.clear();
	
	s_data->num_layers = 0;
	s_data->num_textures = 0;
	s_data->sorted = false;
	s_data->finished = false;
}

bool Rendering_manager::get_finished()
{
	return s_data->finished;
}

void Rendering_manager::reset()
{
	s_data->queue.reset_queue();
}
