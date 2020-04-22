#pragma once

#include <string>

class Texture 
{
private:
	unsigned m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalImageBuffer;
	unsigned m_Width, m_Height, m_BitsPerPixel;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned slot = 0) const;
	void Unbind() const;

	unsigned GetWidth() const;
	unsigned GetHeight() const;
};