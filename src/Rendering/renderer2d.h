#pragma once

//#include "Texture.h"
#include <glm/glm.hpp>

#include <string>

#include "SubTexture.h"
#include "Shader.h"

#include "Orthographic_camera.h"

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

struct Textrender_param
{
	glm::vec4 color = glm::vec4(1.0f);
	std::string text;
	float scale;
	bool centered;

	int layer = 0;
};

class renderer2d
{
public:

	~renderer2d();

	static void Init();
	static void Shutdown();

	static void Begin_Scene(const Orthographic_camera& camera);
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
	
	static void draw_text(Textrender_param& render_data, glm::vec2& position);

	static void update_quad_shader(std::string shader);
	static void update_circle_shader(std::string shader);
	static void update_line_shader(std::string shader);


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

	static std::shared_ptr<SubTexture_Library> get_subtexture_library();
	static std::shared_ptr<Texture_Library> get_texture_library();
	static std::shared_ptr<Shader_Library> get_shader_library();

	static float get_line_width();
	static void set_line_width(float width);

private:

	static float get_texture_index(std::string texture);

	static void Bind_Texture(std::string texture, uint32_t slot);
	static void Unbind_Texture(std::string texture, uint32_t slot);

	static void Flush_Quads();
	static void Flush_Circles();
	static void Flush_Lines();
	static void Flush_Text();


	static void m_draw_quad(render_object& quad_obj);
	static void m_draw_circle(render_object& circle_obj);
	static void m_draw_line(render_object& line_obj);
	static void m_draw_text(render_object& text_obj);

	static void Next_Batch();
	static void Start_Batch();
};

