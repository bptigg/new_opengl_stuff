#include "Framebuffer.h"

#include "GLFW/glfw3.h"
#include <sstream>

static const uint32_t s_max_frame_buffer_size = 8192;

namespace Utils {

    static GLenum Texture_Target(bool multisampled)
    {
        return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void Create_Textures(bool multisampled, uint32_t* out_id, uint32_t count)
    {
        GlCall(glCreateTextures(Texture_Target(multisampled), count, out_id));
    }

    static void Bind_Texture(bool multisampled, uint32_t id)
    {
        glBindTexture(Texture_Target(multisampled), id);
    }

    static void Attach_Color_Texture(uint32_t id, int samples, GLenum internal_Format, GLenum format, uint32_t width, uint32_t height, int index)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            GlCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_Format, width, height, GL_FALSE));
        }
        else
        {
            GlCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_Format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr));

            GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
            GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        }

        GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, Texture_Target(multisampled), id, 0));
    }

    static void Attach_Depth_Texture(uint32_t id, int samples, GLenum format, GLenum attachment_Type, uint32_t width, uint32_t height)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        }
        else
        {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_Type, Texture_Target(multisampled), id, 0);
    }

    static bool Is_depth_format(FramebufferTextureFormat format)
    {
        switch (format)
        {
        case FramebufferTextureFormat::DEPTH24STENCIL8:
            return true;
        }

        return false;
    }

    static GLenum Texture_Format_To_Gl(FramebufferTextureFormat format)
    {
        switch (format)
        {
        case FramebufferTextureFormat::RGBA8:       
            return GL_RGBA8;
        case FramebufferTextureFormat::RED_INTEGER: 
            return GL_RED_INTEGER;
        }

        return 0;
    }
}

Framebuffer::Framebuffer(const Framebufferspec& spec)
    :m_specification(spec)
{
    Invalidate();
}

Framebuffer::~Framebuffer()
{
    GlCall(glDeleteFramebuffers(1, &m_renderer_id));
}

void Framebuffer::Invalidate()
{
    if (m_renderer_id)
    {
        GlCall(glDeleteFramebuffers(1, &m_renderer_id));
        GlCall(glDeleteTextures(m_color_attachments.size(), m_color_attachments.data()));
        GlCall(glDeleteTextures(1, &m_depth_attachment));

        m_color_attachments.clear();
        m_depth_attachment = 0;
    }

    GlCall(glCreateFramebuffers(1, &m_renderer_id));
    GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id));

    bool multisample = m_specification.samples > 1;

    if (m_color_attachment_specifications.size())
    {
        m_color_attachments.resize(m_color_attachment_specifications.size());
        Utils::Create_Textures(multisample, m_color_attachments.data(), m_color_attachments.size());

        for (size_t i = 0; i < m_color_attachments.size(); i++)
        {
            Utils::Bind_Texture(multisample, m_color_attachments[i]);
            switch (m_color_attachment_specifications[i].TextureFormat)
            {
            case FramebufferTextureFormat::RGBA8:
                Utils::Attach_Color_Texture(m_color_attachments[i], m_specification.samples, GL_RGBA8, GL_RGBA, m_specification.width, m_specification.height, i);
                break;
            case FramebufferTextureFormat::RED_INTEGER:
                Utils::Attach_Color_Texture(m_color_attachments[i], m_specification.samples, GL_R32I, GL_RED_INTEGER, m_specification.width, m_specification.height, i);
                break;
            default:
                break;
            }
        }

    }

    if (m_depth_attachment_specfication.TextureFormat != FramebufferTextureFormat::None)
    {
        Utils::Create_Textures(multisample, &m_depth_attachment, 1);
        Utils::Bind_Texture(multisample, m_depth_attachment);
        switch (m_depth_attachment_specfication.TextureFormat)
        {
        case FramebufferTextureFormat::DEPTH24STENCIL8:
            Utils::Attach_Depth_Texture(m_depth_attachment, m_specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specification.width, m_specification.height);
            break;
        }
    }

    if (m_color_attachments.size() > 1)
    {
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_color_attachments.size(), buffers);
    }
    else if (m_color_attachments.empty())
    {
        glDrawBuffer(GL_NONE);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        Log::crit("Framebuffer is incomplete", __FILE__, __LINE__);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0 || width > s_max_frame_buffer_size || height > s_max_frame_buffer_size)
    {
        std::stringstream message;
        message << "Attempted to resize framebuffer to " << width << " , " << height;
        Log::warning(message.str().c_str());
    }

    m_specification.width = width;
    m_specification.height = height;

    Invalidate();
}

int Framebuffer::Read_Pixel(uint32_t attachment_index, int x, int y)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
    int pixel_data;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);
    return pixel_data;
}

void Framebuffer::clear_attachment(uint32_t attachment_index, int value)
{
    auto& spec = m_color_attachment_specifications[attachment_index];
    glClearTexImage(m_color_attachments[attachment_index], 0, Utils::Texture_Format_To_Gl(spec.TextureFormat), GL_INT, &value);
}

std::shared_ptr<Framebuffer> Framebuffer::Create(const Framebufferspec& spec)
{
    return std::make_shared<Framebuffer>(spec);
}
