#pragma once

#include <glm/glm.hpp>
#include <string>

#include <map>
#include <array>
#include <memory>
#include <vector>

enum class render_type
{
	QUAD = 0,
	CIRCLE,
	LINE,
	TEXT,
	GEOMETRY,
	DEFAULT
};

extern struct Geometry_Vertex;
extern class Index_Buffer;

struct render_object
{
	render_type type = render_type::DEFAULT;
	std::array<bool, 2> FlipTexture = {false, false}; //Flip Horizontly Flip Vertically 

	//All types
	glm::vec4 color = glm::vec4(1.0f);
	int layer = 0;

	//QUAD and CIRCLE and GEOMETRY
	glm::mat4 transform = glm::mat4(1.0f);

	//QUAD and GEOMETRY only
	std::string Texture = "";
	float tiling_factor = 1.0f;

	//CIRCLE only 
	float thickness = 1.0f;
	float fade = 0.005f;

	//Line only
	glm::vec3 p0 = glm::vec3(0.0f);
	glm::vec3 p1 = glm::vec3(0.0f);

	//Text only
	glm::vec3 pos = glm::vec3(0.0f);
	float scale = 0.0f;
	glm::vec2 size = glm::vec2(1.0f);
	std::string text;
	bool centered;

	//GEOMETRY only
	std::vector<Geometry_Vertex> verticies;
	std::shared_ptr<Index_Buffer> ib;
};

class render_queue //Batch by layer, then texture
{
private:
	std::map<unsigned int, std::map<std::string, std::vector<std::shared_ptr<render_object>>>> m_queue;

	std::vector<uint32_t> m_active_layers;
	std::vector<std::string> m_active_textures;
	std::array<uint32_t, 2> m_min_max;
public:
	void push(std::shared_ptr<render_object>& object);

	void reset_queue();

	friend class Rendering_manager;
};

class Rendering_manager
{
public:
	static void Init();
	static void Shutdown();
	static void submit_queue_object(std::shared_ptr<render_object>& object);
	static std::vector<std::shared_ptr<render_object>> flush();
	static bool get_finished();
	static void reset();

};

