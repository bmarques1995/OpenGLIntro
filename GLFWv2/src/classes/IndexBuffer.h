#pragma once


#include <GL/glew.h>

class IndexBuffer 
{
private:
	unsigned m_RendererID;
	unsigned m_Count;
public:
	IndexBuffer(unsigned* data, unsigned count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned GetNumberOfMembers() const;
};