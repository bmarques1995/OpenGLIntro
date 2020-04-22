#include "Texture.h"

#include "ErrorHandler.h"
#include <GL/glew.h>
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalImageBuffer(nullptr),
	m_Width(0), m_Height(0), m_BitsPerPixel(0)
{
	stbi_set_flip_vertically_on_load(1);
	int width, height, bpp;
	
	m_LocalImageBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
	
	m_Width = width;
	m_Height = height;
	m_BitsPerPixel = bpp;

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall
	(
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA8,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			m_LocalImageBuffer
		)
	);
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	
	if (m_LocalImageBuffer)
		stbi_image_free(m_LocalImageBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned Texture::GetWidth() const
{
	return m_Width;
}

unsigned Texture::GetHeight() const
{
	return m_Height;
}
