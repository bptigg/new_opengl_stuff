#pragma once

#include "glDebug.h"

#include <memory>
#include <vector>

enum class FramebufferTextureFormat
{
	None = 0,

	// Color
	RGBA8,
	RED_INTEGER,

	// Depth/stencil
	DEPTH24STENCIL8,

	// Defaults
	Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format)
		: TextureFormat(format) {}

	FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
		: Attachments(attachments) {}

	std::vector<FramebufferTextureSpecification> Attachments;
};

struct Framebufferspec
{
	uint32_t width = 0, height = 0;
	FramebufferAttachmentSpecification Attachments;
	uint32_t samples = 1;

	bool swap_chain_target = false;
};

class Framebuffer
{
public:

	Framebuffer(const Framebufferspec& spec);
	~Framebuffer();

	void Invalidate();

	void Bind();
	void Unbind();

	void Resize(uint32_t width, uint32_t height);
	int Read_Pixel(uint32_t attachment_index, int x, int y);

	void clear_attachment(uint32_t attachment_index, int value);
	
	uint32_t get_color_attachment_renderer_id(uint32_t index = 0) const {
		if (index > m_color_attachments.size()) {
			Log::error("Index out of range", __FILE__, __LINE__);
		}
		return m_color_attachments[index];
	}

	inline const Framebufferspec& GetSpec() const { return m_specification; };

	static std::shared_ptr<Framebuffer> Create(const Framebufferspec&);

private:
	uint32_t m_renderer_id = 0;
	Framebufferspec m_specification;

	std::vector<FramebufferTextureSpecification> m_color_attachment_specifications;
	FramebufferTextureSpecification m_depth_attachment_specfication = FramebufferTextureFormat::None;

	std::vector<uint32_t> m_color_attachments;
	uint32_t m_depth_attachment = 0;

public:
	uint32_t get_id() { return m_renderer_id; }
};
