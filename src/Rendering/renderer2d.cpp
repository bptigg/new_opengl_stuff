#include "renderer2d.h"

#include "Vertex.h"
#include "Rendering_manager.h"
#include "SubTexture.h"
#include "Shader.h"

#include "Vertex_Array.h"
#include "Index_Buffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>

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

	float Line_Width = 2.0f;

	std::array<std::string, MAX_TEXTURE_SLOTS> texture_slots;
	uint32_t CurrentSlotIndex = 1; //0 - white
	unsigned int white_texture;

	glm::vec4 Quad_Vertex_Positions[4];
	
	renderer2d::Statistics stats;

	bool initlized = false;
};

static render_data s_data;
static Texture_Library* s_TexLibary = nullptr;
static SubTexture_Library* s_SubTexLibrary = nullptr;
static Shader_Library* s_ShaderLibrary = nullptr;


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

	s_TexLibary = new Texture_Library;
	s_SubTexLibrary = new SubTexture_Library;
	s_ShaderLibrary = new Shader_Library;

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

		//s_data.characters[c] = character;
		//s_data.characters[c].tex_id = Texture::Create_Texture(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
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

	int32_t samplers[MAX_TEXTURE_SLOTS];
	for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
	{
		samplers[i] = i;
	}

	//Load shaders

	s_data.Quad_Vertex_Positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_data.Quad_Vertex_Positions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
	s_data.Quad_Vertex_Positions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
	s_data.Quad_Vertex_Positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
}

void renderer2d::Shutdown()
{
	delete s_TexLibary;
	delete s_SubTexLibrary;
	delete s_ShaderLibrary;

	//delete s_data.Quad_IB;
}

void renderer2d::Begin_Scene()
{
}

void renderer2d::End_Scene()
{
}

void renderer2d::draw_quad(const glm::vec2& position, QUADrender_param& render_data)
{
	renderer2d::draw_qaud({ position.x, position.y, 0.0f }, render_data);
}

void renderer2d::draw_qaud(const glm::vec3& position, QUADrender_param& render_data)
{
	glm::mat4 transform = glm::mat4(1.0f);
	if (render_data.rotation == 0.0f)
	{
		transform = glm::translate(glm::mat4(1.0f), position) 
				  * glm::scale(glm::mat4(1.0f), { render_data.size.x, render_data.size.y, 1.0f });
	}
	else
	{
		render_data.rotation = (render_data.rotation > 2 * glm::pi<float>()) ? glm::radians(render_data.rotation) : render_data.rotation;
		transform = glm::translate(glm::mat4(1.0f), position) 
				  * glm::rotate(glm::mat4(1.0f), render_data.rotation, {0.0f, 0.0f,1.0f})
				  * glm::scale(glm::mat4(1.0f), { render_data.size.x, render_data.size.y, 1.0f });
	}

	renderer2d::draw_quad(transform, render_data);
}

void renderer2d::draw_quad(const glm::mat4& transform, QUADrender_param& render_data)
{
	//send it off to the render manager
	render_object quad;
	quad.type = render_type::QUAD;
	quad.transform = transform;
	quad.color = render_data.color;
	quad.Texture = render_data.Texture;
	quad.tiling_factor = render_data.tiling_factor;
	quad.layer = render_data.layer;

	Rendering_manager::submit_queue_object(quad);
}

void renderer2d::draw_circle(CIRCLErender_param& render_data)
{
}

void renderer2d::draw_line(LINErender_param& render_data)
{
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
	
	float texture_index = 0.0f;
	for (uint32_t i = 0; i < s_data.CurrentSlotIndex; i++)
	{
		if (s_data.texture_slots[i] == texture)
		{
			texture_index = (float)i;
			break;
		}
	}

	if (texture_index == 0.0f)
	{
		if (s_data.CurrentSlotIndex >= s_data.max_texture_slots)
		{
			//Next_Batch();
		}

		texture_index = (float)s_data.CurrentSlotIndex;
		s_data.texture_slots[s_data.CurrentSlotIndex] = texture;
		s_data.CurrentSlotIndex++;
	}

	return texture_index;
}

void renderer2d::Bind_Texture(std::string texture, uint32_t slot)
{
	std::shared_ptr<Texture_Data> data = s_TexLibary->get(texture);
	if (data->bound == false)
	{
		Texture::bind(data->texture_id, slot);
		data->bound = true;
		data->slot = slot;
	}
	else if (data->bound == true && slot != data->slot)
	{
		Texture::unbind(data->slot);
		Texture::bind(data->texture_id, slot);
		data->slot = slot;
	}
}

void renderer2d::Unbind_Texture(std::string texture, uint32_t slot)
{
	std::shared_ptr<Texture_Data> data = s_TexLibary->get(texture);
	if (data->bound == true && data->slot == slot)
	{
		Texture::unbind(slot);
		data->bound = false;
		data->slot = 0;
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

void renderer2d::m_draw_quad(render_object& quad_obj)
{
	constexpr size_t quad_vertex_count = 4;
	
	std::string parent_texture = s_SubTexLibrary->get(quad_obj.Texture)->get_parent_texture();
	const glm::vec2* texture_coords = s_SubTexLibrary->get(quad_obj.Texture)->get_tex_coords();
	float texture_index = get_texture_index(parent_texture);

	if (s_data.Quad_index_count + 6 >= s_data.max_indicies + 1)
	{
		//NextBatch();
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

	s_data.CurrentSlotIndex = 1;
}