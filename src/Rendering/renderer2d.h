#pragma once

//#include "Texture.h"
#include <glm/glm.hpp>

#include <string>

#include "SubTexture.h"
#include "Shader.h"

struct render_object;

struct QUADrender_param
{
	glm::vec2 size = glm::vec2(0.0f);
	glm::vec4 color = glm::vec4(1.0f);
	std::string Texture = "";
	float tiling_factor = 1.0f;
	float rotation = 0.0f;

	int layer = 0; // z depth 
};

struct CIRCLErender_param
{
	glm::mat4 transform;
	glm::vec4 color;
	float thickness = 1.0f;
	float fade = 0.005f;

	int layer = 0;
};

struct LINErender_param
{
	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec4 color;

	int layer = 0;
};

class renderer2d
{
public:

	~renderer2d();

	static void Init();
	static void Shutdown();

	static void Begin_Scene();
	static void End_Scene();
	static void Flush();
	static void draw();
	static void clear();

	//Primative types
	static void draw_quad(const glm::vec2& posiiton, QUADrender_param& render_data);
	static void draw_quad(const glm::vec3& position, QUADrender_param& render_data);
	static void draw_quad(const glm::mat4& transform, QUADrender_param& render_data);

	static void draw_circle(CIRCLErender_param& render_data);
	static void draw_line(LINErender_param& render_data);

	//static void update_quad_shader(std::string shader);
	//static void update_circle_shader(std::string shader);
	//static void update_line_shader(std::string shader);


public:
	struct Statistics
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;

		uint32_t get_vertex_count() const { return QuadCount * 4; }
		uint32_t get_index_count() const { return QuadCount * 6; }
	};

	static void reset_stats();
	static Statistics get_stats();

	static void enable_blending();
	static void disable_blending();

	static SubTexture_Library* get_subtexture_library();
	static Texture_Library* get_texture_library();
	static Shader_Library* get_shader_library();

private:

	static float get_texture_index(std::string texture);

	static void Bind_Texture(std::string texture, uint32_t slot);
	static void Unbind_Texture(std::string texture, uint32_t slot);

	static void Flush_Quads();
	static void Flush_Circles();
	static void Flush_Lines();


	static void m_draw_quad(render_object& quad_obj);
	static void m_draw_circle(render_object& circle_obj);

	static void Next_Batch();
	static void Start_Batch();


};

