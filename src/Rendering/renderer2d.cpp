#include "renderer2d.h"

#include "Vertex.h"
#include "Rendering_manager.h"
//#include "SubTexture.h"
//#include "Shader.h"

#include "Vertex_Array.h"
#include "Index_Buffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <cmath>

#include <ft2build.h>
#include FT_FREETYPE_H

static const size_t MAX_TEXTURE_SLOTS = 32;

struct render_data
{
	static const uint32_t max_quads = 10000;
	static const uint32_t max_verticies = max_quads * 4;
	static const uint32_t max_indicies = max_quads * 6;
	uint32_t max_texture_slots = 0;

	uint32_t Quad_index_count = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;
	std::shared_ptr<Vertex_Array> Qaud_VA0;
	std::shared_ptr<Vertex_Buffer> Quad_VB;
	std::shared_ptr<shader> Quad_shader;

	std::shared_ptr<Index_Buffer> Quad_IB;

	uint32_t Circle_index_count = 0;
	CircleVertex* CircleVertexBufferBase = nullptr;
	CircleVertex* CircleVertexBufferPtr = nullptr;
	std::shared_ptr<Vertex_Array> Circle_VA0;
	std::shared_ptr<Vertex_Buffer> Circle_VB;
	std::shared_ptr<shader> Circle_shader;
	
	uint32_t Line_index_count = 0;
	LineVertex* LineVertexBufferBase = nullptr;
	LineVertex* LineVertexBufferPtr = nullptr;
	std::shared_ptr<Vertex_Array> Line_VA0;
	std::shared_ptr<Vertex_Buffer> Line_VB;
	std::shared_ptr<shader> Line_shader;

	uint32_t Text_index_count = 0;
	TextVertex* TextVertexBufferBase = nullptr;
	TextVertex* TextVertexBufferPtr = nullptr;
	std::shared_ptr<Vertex_Array> Text_VAO;
	std::shared_ptr<Vertex_Buffer> Text_VB;
	std::shared_ptr<shader> Text_shader;

	float Line_Width = 2.0f;

	std::array<std::string, MAX_TEXTURE_SLOTS> texture_slots;
	std::unordered_map<int, std::string> assigned_slots;
	uint32_t CurrentSlotIndex = 1; //0 - white
	unsigned int white_texture;

	glm::vec4 Quad_Vertex_Positions[4];


	std::unordered_map<char, Character> characters;
	
	renderer2d::Statistics stats;

	bool initlized = false;
};

static render_data s_data;
static std::shared_ptr<Texture_Library> s_TexLibary = nullptr;
static std::shared_ptr<SubTexture_Library> s_SubTexLibrary = nullptr;
static std::shared_ptr<Shader_Library> s_ShaderLibrary = nullptr;


renderer2d::~renderer2d()
{
	if (s_data.initlized)
	{
		Shutdown();
	}
}

void renderer2d::Init()
{
	if (s_data.initlized)
	{
		Log::error("RENDERER ALREADY INITLIZED", __FILE__, __LINE__);
		return;
	}

	s_TexLibary = std::shared_ptr<Texture_Library>(new Texture_Library);
	s_SubTexLibrary = std::shared_ptr<SubTexture_Library>(new SubTexture_Library);
	s_ShaderLibrary = std::shared_ptr<Shader_Library>(new Shader_Library);
	Rendering_manager::Init();

	GLint max_slots;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_slots);
	s_data.max_texture_slots = (uint32_t)max_slots;
	s_data.max_texture_slots = (s_data.max_texture_slots > MAX_TEXTURE_SLOTS) ? MAX_TEXTURE_SLOTS : s_data.max_texture_slots;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		Log::crit("FREETYPE: Could not initlized free", __FILE__, __LINE__);
	}

	FT_Face face;
	if (FT_New_Face(ft, "res/fonts/HPSimplified_Rg.ttf", 0, &face))
	{
		Log::crit("FREETYPE: Failed to load font", __FILE__, __LINE__);
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		Log::crit("FREETYPE: Failed to load glyph", __FILE__, __LINE__);
	}
	GlCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			Log::warning("FREETYPE: Failed to load glyph");
			continue;
		}

		Character character = {
			0,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(uint16_t)face->glyph->advance.x,
		};

		s_data.characters[c] = character;
		s_data.characters[c].tex_id = Texture::Create_Texture(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);

		std::shared_ptr<Texture_Data> chr = std::make_shared<Texture_Data>(Texture_Data());
		chr->texture_id = s_data.characters[c].tex_id;
		chr->bound = false;
		chr->slot = 0;
		chr->alive = true;
		std::string name(1, c);
		s_TexLibary->Add(name, chr);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	Log::info("Text initialised");
	s_data.Quad_VB	=std::shared_ptr<Vertex_Buffer>(new Vertex_Buffer(s_data.max_verticies * sizeof(QuadVertex)));
	s_data.Qaud_VA0 = std::shared_ptr<Vertex_Array>(new Vertex_Array());
	{
		Vertex_Buffer_Layout quad_layout;
		quad_layout.Push<float>(3);
		quad_layout.Push<float>(4);
		quad_layout.Push<float>(2);
		quad_layout.Push<float>(1);
		quad_layout.Push<float>(1);
		s_data.Qaud_VA0->add_buffer(*s_data.Quad_VB, quad_layout);
	}

	s_data.Quad_IB = std::shared_ptr<Index_Buffer>(new Index_Buffer(s_data.max_indicies));
	s_data.Qaud_VA0->add_index_buffer(s_data.Quad_IB);
	s_data.QuadVertexBufferBase = new QuadVertex[s_data.max_verticies];

	s_data.Circle_VB  = std::shared_ptr<Vertex_Buffer>(new Vertex_Buffer(s_data.max_verticies * sizeof(CircleVertex)));
	s_data.Circle_VA0 = std::shared_ptr<Vertex_Array>(new Vertex_Array());
	{
		Vertex_Buffer_Layout circle_layout;
		circle_layout.Push<float>(3);
		circle_layout.Push<float>(3);
		circle_layout.Push<float>(4);
		circle_layout.Push<float>(1);
		circle_layout.Push<float>(1);
		s_data.Circle_VA0->add_buffer(*s_data.Circle_VB, circle_layout);
	}

	s_data.Circle_VA0->add_index_buffer(s_data.Quad_IB);
	s_data.CircleVertexBufferBase = new CircleVertex[s_data.max_verticies];

	s_data.Line_VB  = std::shared_ptr<Vertex_Buffer>(new Vertex_Buffer(s_data.max_verticies * sizeof(LineVertex)));
	s_data.Line_VA0 = std::shared_ptr<Vertex_Array>(new Vertex_Array());
	{
		Vertex_Buffer_Layout line_layout;
		line_layout.Push<float>(3);
		line_layout.Push<float>(4);
		s_data.Line_VA0->add_buffer(*s_data.Line_VB, line_layout);
	}
	s_data.LineVertexBufferBase = new LineVertex[s_data.max_verticies];

	s_data.Text_VB = std::shared_ptr<Vertex_Buffer>(new Vertex_Buffer(s_data.max_verticies * sizeof(TextVertex)));
	s_data.Text_VAO = std::shared_ptr<Vertex_Array>(new Vertex_Array());
	{
		Vertex_Buffer_Layout text_layout;
		text_layout.Push<float>(3);
		text_layout.Push<float>(4);
		text_layout.Push<float>(2);
		text_layout.Push<float>(1);
		s_data.Text_VAO->add_buffer(*s_data.Text_VB, text_layout);
	}
	s_data.Text_VAO->add_index_buffer(s_data.Quad_IB);
	s_data.TextVertexBufferBase = new TextVertex[s_data.max_verticies];

	unsigned int color = 0xffffffff;
	GLuint white_texture = Texture::Create_Texture(1, 1, color, s_data.white_texture);
	
	std::shared_ptr<Texture_Data> white = std::make_shared<Texture_Data>(Texture_Data());
	white->texture_id = white_texture;
	white->size = { 1,1 };
	white->bound = false;
	white->slot = 0;
	white->alive = true;

	std::string white_name = "m_white";
	s_TexLibary->Add(white_name, white);
	s_SubTexLibrary->create("s_white", white_name, *white, { 0,0 }, { 1,1 }, { 1,1 });
	s_data.texture_slots[0] = white_name;

	int samplers[MAX_TEXTURE_SLOTS];
	for (int i = 0; i < MAX_TEXTURE_SLOTS; i++)
	{
		samplers[i] = i;
	}

	//Load shaders
	s_ShaderLibrary->Load("Quad", "res/shaders/Quad.glsl");
	s_data.Quad_shader = s_ShaderLibrary->get("Quad");

	s_ShaderLibrary->Load("Circle", "res/shaders/Circle.glsl");
	s_data.Circle_shader = s_ShaderLibrary->get("Circle");

	s_ShaderLibrary->Load("Line", "res/shaders/Line.glsl");
	s_data.Line_shader = s_ShaderLibrary->get("Line");

	s_ShaderLibrary->Load("Text", "res/shaders/Text.glsl");
	s_data.Text_shader = s_ShaderLibrary->get("Text");

	//temp thing 

	glm::mat4 mvp = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);

	//s_data.Quad_shader->set_uniform_mat_4f("u_view_proj", mvp);
	s_data.Quad_shader->set_uniform_1iv("u_textures", 32, samplers);

	//s_data.Circle_shader->set_uniform_mat_4f("u_view_proj", mvp);
	//s_data.Line_shader->set_uniform_mat_4f("u_view_proj", mvp);
	
	//s_data.Text_shader->set_uniform_mat_4f("u_view_proj", mvp);
	s_data.Text_shader->set_uniform_1iv("u_textures", 32, samplers);
	

	s_data.Quad_Vertex_Positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_data.Quad_Vertex_Positions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
	s_data.Quad_Vertex_Positions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
	s_data.Quad_Vertex_Positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
}

void renderer2d::Shutdown()
{
	//delete s_TexLibary;
	//delete s_SubTexLibrary;
	//delete s_ShaderLibrary;

	s_data.Quad_IB->shutdown();
	s_data.Quad_VB->shutdown();
	s_data.Qaud_VA0->shutdown();

	s_data.Circle_VB->shutdown();
	s_data.Circle_VA0->shutdown();

	s_data.Line_VB->shutdown();
	s_data.Line_VA0->shutdown();

	s_data.Text_VB->shutdown();
	s_data.Text_VAO->shutdown();

	delete[] s_data.QuadVertexBufferBase;
	delete[] s_data.CircleVertexBufferBase;
	delete[] s_data.LineVertexBufferBase;
	delete[] s_data.TextVertexBufferBase;

	Rendering_manager::Shutdown();
	s_ShaderLibrary->shutdown();
	s_TexLibary->shutdown();

	//delete s_data.Quad_IB;
	Log::info("RENDERER 2D SHUTDOWN");
}

void renderer2d::Begin_Scene(const Orthographic_camera& camera)
{
	glm::mat4 mvp = camera.Get_View_Projection_Matrix();
	
	s_data.Quad_shader->set_uniform_mat_4f("u_view_proj", mvp);
	s_data.Circle_shader->set_uniform_mat_4f("u_view_proj", mvp);
	s_data.Line_shader->set_uniform_mat_4f("u_view_proj", mvp);
	s_data.Text_shader->set_uniform_mat_4f("u_view_proj", mvp);
}

void renderer2d::End_Scene()
{
	draw();
}

void renderer2d::draw_quad(const glm::vec2& position, QUADrender_param& render_data)
{
	renderer2d::draw_quad({ position.x, position.y, 0.0f }, render_data);
}

void renderer2d::draw_quad(const glm::vec3& position, QUADrender_param& render_data)
{
	glm::mat4 transform = glm::mat4(1.0f);
	if (render_data.rotation == 0.0f)
	{
		transform = glm::translate(glm::mat4(1.0f), position) 
				  * glm::scale(glm::mat4(1.0f), { render_data.size.x, render_data.size.y, 1.0f });
	}
	else
	{
		render_data.rotation = (render_data.rotation > 2 * glm::pi<float>()) ? std::fmod(render_data.rotation,glm::pi<float>()) : render_data.rotation;
		transform = glm::translate(glm::mat4(1.0f), position) 
				  * glm::rotate(glm::mat4(1.0f), render_data.rotation, {0.0f, 0.0f,1.0f})
				  * glm::scale(glm::mat4(1.0f), { render_data.size.x, render_data.size.y, 1.0f });
	}

	renderer2d::draw_quad(transform, render_data);
}

void renderer2d::draw_quad(const glm::mat4& transform, QUADrender_param& render_data)
{
	//send it off to the render manager
	std::shared_ptr<render_object>quad(new render_object);
	quad->type = render_type::QUAD;
	quad->transform = transform;
	quad->color = render_data.color;
	quad->Texture = (render_data.Texture != "") ? render_data.Texture : "s_white";
	quad->tiling_factor = render_data.tiling_factor;
	quad->layer = render_data.layer;

	Rendering_manager::submit_queue_object(quad);
}

void renderer2d::draw_circle(CIRCLErender_param& render_data)
{
	std::shared_ptr<render_object>circle(new render_object);
	circle->type = render_type::CIRCLE;
	circle->transform = render_data.transform;
	circle->color = render_data.color;
	circle->thickness = render_data.thickness;
	circle->fade = render_data.fade;
	circle->Texture = "s_white";
	circle->layer = render_data.layer;

	Rendering_manager::submit_queue_object(circle);
}

void renderer2d::draw_line(LINErender_param& render_data)
{
	std::shared_ptr<render_object>line(new render_object);
	line->type = render_type::LINE;
	line->p0 = render_data.p0;
	line->p1 = render_data.p1;
	line->Texture = "s_white";
	line->color = render_data.color;
	line->layer = render_data.layer;

	Rendering_manager::submit_queue_object(line);
}

void renderer2d::draw_text(Textrender_param& render_data, glm::vec2& position)
{
	std::shared_ptr<render_object>text(new render_object);

	float width		= 0.0f;
	float height	= 0.0f;
	float min_y		= 0.0f;
	float new_scale = render_data.scale / 100;

	for (std::string::const_iterator c = render_data.text.begin(); c != render_data.text.end(); c++)
	{
		Character* ch = &s_data.characters[*c];
		width += (ch->advance >> 6) * new_scale;
		float h = ch->bearing.y * new_scale;
		if (h > height)
		{
			height = h;
		}
		float y = (ch->bearing.y - ch->size.y) * new_scale;
		if (y > min_y)
		{
			min_y = y;
		}
	}

	text->centered = render_data.centered;
	text->pos = glm::vec3(position,1.0f);
	text->color = render_data.color;
	text->text = render_data.text;
	text->scale = new_scale;
	text->size = { width, height - min_y };
	text->layer = render_data.layer;
	text->type = render_type::TEXT;
	text->Texture = "Text";

	Rendering_manager::submit_queue_object(text);
}

void renderer2d::update_quad_shader(std::string shader)
{
	s_data.Quad_shader = s_ShaderLibrary->get(shader);
}

void renderer2d::update_circle_shader(std::string shader)
{
	s_data.Circle_shader = s_ShaderLibrary->get(shader);
}

void renderer2d::update_line_shader(std::string shader)
{
	s_data.Line_shader = s_ShaderLibrary->get(shader);
}

void renderer2d::reset_stats()
{
	memset(&s_data.stats, 0, sizeof(Statistics));
}

renderer2d::Statistics renderer2d::get_stats()
{
	return s_data.stats;
}

float renderer2d::get_texture_index(std::string texture)
{
	
	float texture_index = -1.0f;
	for (uint32_t i = 0; i < s_data.CurrentSlotIndex; i++)
	{
		if (s_data.texture_slots[i] == texture)
		{
			texture_index = (float)i;
			break;
		}
	}

	if (texture_index == -1.0f)
	{
		if (s_data.CurrentSlotIndex >= s_data.max_texture_slots)
		{
			Next_Batch();
		}

		texture_index = (float)s_data.CurrentSlotIndex;
		s_data.texture_slots[s_data.CurrentSlotIndex] = texture;
		s_data.CurrentSlotIndex++;
	}

	return texture_index;
}

float renderer2d::bind_texture(std::string texture)
{
	float index = get_texture_index(texture);
	Bind_Texture(texture, index);
	return index;
}

void renderer2d::Bind_Texture(std::string texture, uint32_t slot)
{
	std::shared_ptr<Texture_Data> data = s_TexLibary->get(texture);
	if (data->bound == false)
	{
		if (s_data.assigned_slots[slot] != "")
		{
			Unbind_Texture(s_data.assigned_slots[slot], slot);
		}

		Texture::bind(data->texture_id, slot, data->multisampled);
		data->bound = true;
		data->slot = slot;

		s_data.assigned_slots[slot] = texture;
	}
	else if (data->bound == true && slot != data->slot)
	{
		if (s_data.assigned_slots[slot] != "")
		{
			Unbind_Texture(s_data.assigned_slots[slot], slot);
		}

		Texture::bind(data->texture_id, slot, data->multisampled);
		data->slot = slot;

	}
	else if (data->always_rebind == true)
	{
		if (s_data.assigned_slots[slot] != "")
		{
			Unbind_Texture(s_data.assigned_slots[slot], slot);
		}

		Texture::bind(data->texture_id, slot, data->multisampled);
		data->bound = true;
		data->slot = slot;

		s_data.assigned_slots[slot] = texture;
	}
}

void renderer2d::unbind_texture(std::string texture)
{
	std::shared_ptr<Texture_Data> data = s_TexLibary->get(texture);
	Unbind_Texture(texture, data->slot);
}

void renderer2d::Unbind_Texture(std::string texture, uint32_t slot)
{
	std::shared_ptr<Texture_Data> data = s_TexLibary->get(texture);
	if (data->bound == true && data->slot == slot)
	{
		Texture::unbind(slot);
		data->bound = false;
		data->slot = 0;
		s_data.assigned_slots[slot] = "";
	}
	else if (data->bound == true && data->slot != slot)
	{
		Log::warning("Trying to unbind a texture that isnt bound to this slot");
	}
	else
	{
		Log::warning("Trying to unbind a texture that isnt bound");
	}
}

void renderer2d::draw()
{
	Start_Batch();
	while (!Rendering_manager::get_finished())
	{
		auto draw_calls = Rendering_manager::flush();
		if (!draw_calls.empty())
		{
			for (int i = 0; i < draw_calls.size(); i++)
			{
				switch (draw_calls[i]->type)
				{
				case render_type::QUAD:
					m_draw_quad(*draw_calls[i]);
					break;
				case render_type::CIRCLE:
					m_draw_circle(*draw_calls[i]);
					break;
				case render_type::LINE:
					m_draw_line(*draw_calls[i]);
					break;
				case render_type::TEXT:
					m_draw_text(*draw_calls[i]);
					break;
				default:
					Log::warning("UNKNOWN DRAW TYPE");
					break;
				}
			}
			Next_Batch();
		}
	}
	Rendering_manager::reset();
}

void renderer2d::Flush()
{
	if (s_data.Quad_index_count)
	{
		Flush_Quads();
	}
	if (s_data.Circle_index_count)
	{
		Flush_Circles();
	}
	if (s_data.Line_index_count)
	{
		Flush_Lines();
	}
	if (s_data.Text_index_count)
	{
		Flush_Text(); //Need to fix the texture binding stuff
	}
}

void renderer2d::Flush_Quads()
{
	uint32_t data_size = (uint32_t)((uint8_t*)s_data.QuadVertexBufferPtr - (uint8_t*)s_data.QuadVertexBufferBase);
	s_data.Quad_VB->add_to_buffer((void*)s_data.QuadVertexBufferBase, data_size);

	for (uint32_t i = 0; i < s_data.CurrentSlotIndex; i++)
	{
		Bind_Texture(s_data.texture_slots[i], i);
	}

	s_data.Quad_shader->bind();
	s_data.Qaud_VA0->bind();
	GlCall(glDrawElements(GL_TRIANGLES, s_data.Quad_index_count, GL_UNSIGNED_INT, nullptr));
	s_data.stats.DrawCalls++;

	s_data.Quad_VB->clear_buffer();
}

void renderer2d::Flush_Circles()
{
	uint32_t data_size = (uint32_t)((uint8_t*)s_data.CircleVertexBufferPtr - (uint8_t*)s_data.CircleVertexBufferBase);
	s_data.Circle_VB->add_to_buffer((void*)s_data.CircleVertexBufferBase, data_size);

	for (uint32_t i = 0; i < s_data.CurrentSlotIndex; i++)
	{
		Bind_Texture(s_data.texture_slots[i], i);
	}

	s_data.Circle_shader->bind();
	s_data.Circle_VA0->bind();
	GlCall(glDrawElements(GL_TRIANGLES, s_data.Circle_index_count, GL_UNSIGNED_INT, nullptr));
	s_data.stats.DrawCalls++;

	s_data.Circle_VB->clear_buffer();
}

void renderer2d::Flush_Lines()
{
	uint32_t data_size = (uint32_t)((uint8_t*)s_data.LineVertexBufferPtr - (uint8_t*)s_data.LineVertexBufferBase);
	s_data.Line_VB->add_to_buffer((void*)s_data.LineVertexBufferBase, data_size);

	for (uint32_t i = 0; i < s_data.CurrentSlotIndex; i++)
	{
		Bind_Texture(s_data.texture_slots[i], i);
	}

	s_data.Line_shader->bind();
	glLineWidth(s_data.Line_Width);
	s_data.Line_VA0->bind();
	GlCall(glDrawArrays(GL_LINES, 0, s_data.Line_index_count));
	s_data.stats.DrawCalls++;

	s_data.Line_VB->clear_buffer();
}

void renderer2d::Flush_Text()
{
	uint32_t data_size = (uint32_t)((uint8_t*)s_data.TextVertexBufferPtr - (uint8_t*)s_data.TextVertexBufferBase);
	s_data.Text_VB->add_to_buffer((void*)s_data.TextVertexBufferBase, data_size);

	for (uint32_t i = 0; i < s_data.CurrentSlotIndex; i++)
	{
		Bind_Texture(s_data.texture_slots[i], i);
	}

	s_data.Text_shader->bind();
	s_data.Text_VAO->bind();
	GlCall(glDrawElements(GL_TRIANGLES, s_data.Text_index_count, GL_UNSIGNED_INT, nullptr));
	s_data.stats.DrawCalls++;

	s_data.Text_VB->clear_buffer();
}

void renderer2d::m_draw_quad(render_object& quad_obj)
{
	constexpr size_t quad_vertex_count = 4;
	
	std::string parent_texture = s_SubTexLibrary->get(quad_obj.Texture)->get_parent_texture();
	const glm::vec2* texture_coords = s_SubTexLibrary->get(quad_obj.Texture)->get_tex_coords();
	float texture_index = get_texture_index(parent_texture);

	if (s_data.Quad_index_count + 6 >= s_data.max_indicies + 1)
	{
		Next_Batch();
	}

	for (size_t i = 0; i < quad_vertex_count; i++)
	{
		s_data.QuadVertexBufferPtr->Position = quad_obj.transform * s_data.Quad_Vertex_Positions[i];
		s_data.QuadVertexBufferPtr->Colour = quad_obj.color;
		s_data.QuadVertexBufferPtr->TexCoord = texture_coords[i];
		s_data.QuadVertexBufferPtr->TexIndex = texture_index;
		s_data.QuadVertexBufferPtr->TilingFactor = quad_obj.tiling_factor;
		s_data.QuadVertexBufferPtr++;
	}

	s_data.Quad_index_count += 6;
	s_data.stats.QuadCount++;
}

void renderer2d::m_draw_circle(render_object& circle_obj)
{
	constexpr size_t circle_vertex_count = 4;

	if (s_data.Quad_index_count + 6 >= s_data.max_indicies + 1)
	{
		Next_Batch();
	}

	for (size_t i = 0; i < circle_vertex_count; i++)
	{
		s_data.CircleVertexBufferPtr->WorldPosition = circle_obj.transform * s_data.Quad_Vertex_Positions[i];
		s_data.CircleVertexBufferPtr->LocalPosition = s_data.Quad_Vertex_Positions[i] * 2.0f;
		s_data.CircleVertexBufferPtr->Colour = circle_obj.color;
		s_data.CircleVertexBufferPtr->thickness = circle_obj.thickness;
		s_data.CircleVertexBufferPtr->fade = circle_obj.fade;
		s_data.CircleVertexBufferPtr++;
	}

	s_data.Circle_index_count += 6;
	s_data.stats.QuadCount++;
}

void renderer2d::m_draw_line(render_object& line_obj)
{
	s_data.LineVertexBufferPtr->Position = line_obj.p0;
	s_data.LineVertexBufferPtr->Colour = line_obj.color;
	s_data.LineVertexBufferPtr++;

	s_data.LineVertexBufferPtr->Position = line_obj.p1;
	s_data.LineVertexBufferPtr->Colour = line_obj.color;
	s_data.LineVertexBufferPtr++;

	s_data.Line_index_count += 2;
}

void renderer2d::m_draw_text(render_object& text_obj)
{
	float x_offset = 0.0f;
	//Start_Batch();
	for (std::string::const_iterator c = text_obj.text.begin(); c != text_obj.text.end(); c++)
	{
		Character* ch = &s_data.characters[*c];
		if (s_data.Text_index_count + 6 >= s_data.max_indicies + 1)
		{
			Next_Batch();
		}

		float tex_id = ch->tex_id;

		float tex_index = get_texture_index(std::string(1,*c));

		glm::vec3 pos = { text_obj.pos.x + (ch->bearing.x * text_obj.scale) + x_offset, text_obj.pos.y + (ch->size.y - ch->bearing.y) * text_obj.scale, 1.0f};
		float w = ch->size.x * text_obj.scale;
		float h = ch->size.y * text_obj.scale;

		glm::vec2 bottom, top;
		if (text_obj.centered)
		{
			bottom.x = -text_obj.size.x / 2.0f;
			bottom.y = -text_obj.size.y / 2.0f;

			top.x = bottom.x + w;
			top.y = bottom.y + h;
		}
		else
		{
			bottom.x = 0.0f;
			bottom.y = 0.0f;

			top.x = w;
			top.y = h;
		}

		s_data.TextVertexBufferPtr->Position = { pos.x + bottom.x, pos.y + top.y, 1.0f };
		s_data.TextVertexBufferPtr->Color = text_obj.color;
		s_data.TextVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_data.TextVertexBufferPtr->TexIndex = tex_index;
		s_data.TextVertexBufferPtr++;

		s_data.TextVertexBufferPtr->Position = { pos.x + top.x, pos.y + top.y, 1.0f };
		s_data.TextVertexBufferPtr->Color = text_obj.color;
		s_data.TextVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_data.TextVertexBufferPtr->TexIndex = tex_index;
		s_data.TextVertexBufferPtr++;

		s_data.TextVertexBufferPtr->Position = { pos.x + top.x, pos.y + bottom.y, 1.0f };
		s_data.TextVertexBufferPtr->Color = text_obj.color;
		s_data.TextVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_data.TextVertexBufferPtr->TexIndex = tex_index;
		s_data.TextVertexBufferPtr++;

		s_data.TextVertexBufferPtr->Position = { pos.x + bottom.x, pos.y + bottom.y, 1.0f };
		s_data.TextVertexBufferPtr->Color = text_obj.color;
		s_data.TextVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_data.TextVertexBufferPtr->TexIndex = tex_index;
		s_data.TextVertexBufferPtr++;

		s_data.Text_index_count += 6;

		x_offset += (ch->advance >> 6) * text_obj.scale;

		Next_Batch();

	}
}

void renderer2d::Next_Batch()
{
	Flush();
	Start_Batch();
}

void renderer2d::Start_Batch()
{
	s_data.Quad_index_count = 0;
	s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;

	s_data.Circle_index_count = 0;
	s_data.CircleVertexBufferPtr = s_data.CircleVertexBufferBase;

	s_data.Line_index_count = 0;
	s_data.LineVertexBufferPtr = s_data.LineVertexBufferBase;

	s_data.Text_index_count = 0;
	s_data.TextVertexBufferPtr = s_data.TextVertexBufferBase;

	s_data.CurrentSlotIndex = 1;
}

void renderer2d::clear()
{
	GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

void renderer2d::enable_blending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void renderer2d::disable_blending()
{
	glDisable(GL_BLEND);
}

std::shared_ptr<SubTexture_Library> renderer2d::get_subtexture_library()
{
	return s_SubTexLibrary;
}

std::shared_ptr<Texture_Library> renderer2d::get_texture_library()
{
	return s_TexLibary;
}

std::shared_ptr<Shader_Library> renderer2d::get_shader_library()
{
	return s_ShaderLibrary;
}

float renderer2d::get_line_width()
{
	return s_data.Line_Width;
}

void renderer2d::set_line_width(float width)
{
	s_data.Line_Width = width;
}
