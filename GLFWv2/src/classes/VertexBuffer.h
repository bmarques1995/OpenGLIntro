#pragma once

#include "ErrorHandler.h"
#include <GL/glew.h>

class VertexBuffer 
{
private:
	unsigned m_RendererID;
public:
	VertexBuffer(const void *data, unsigned size);
	~VertexBuffer();

	void Bind();
	void Unbind();
};