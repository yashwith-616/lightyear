#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLFramebuffer.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad/glad.h>
LY_DISABLE_WARNINGS_POP

namespace ly::renderer {

OpenGlFramebuffer::OpenGlFramebuffer(FramebufferSpecification const& spec) : m_specification(spec) {
    invalidate();
}

OpenGlFramebuffer::~OpenGlFramebuffer() {
    glDeleteFramebuffers(1, &m_renderId);
    glDeleteTextures(1, &m_colorAttachment);
    glDeleteTextures(1, &m_depthAttachment);
}

void OpenGlFramebuffer::invalidate() {
    if (m_renderId > 0) {
        glDeleteFramebuffers(1, &m_renderId);
        glDeleteTextures(1, &m_colorAttachment);
        glDeleteTextures(1, &m_depthAttachment);
    }

    GLsizei const& width  = narrowCast<GLsizei>(m_specification.width);
    GLsizei const& height = narrowCast<GLsizei>(m_specification.height);

    glViewport(0, 0, width, height);

    // Create framebuffer
    glCreateFramebuffers(1, &m_renderId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderId);

    // Color Attachment
    glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

    // Depth Attachment
    glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

    LY_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGlFramebuffer::resize(uint32_t width, uint32_t height) {
    LY_CORE_ASSERT((width > 0 && height > 0), "Invalid Framebuffer resize width {} & height {}", width, height);

    m_specification.width  = width;
    m_specification.height = height;
    invalidate();  // Recreate framebuffer with new size
}

void OpenGlFramebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderId);
}

void OpenGlFramebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}  // namespace ly::renderer