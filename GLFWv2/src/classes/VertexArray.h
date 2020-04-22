#pragma once

#include "ErrorHandler.h"
#include "VertexBufferLayout.h"
#include <GL/glew.h>
#include "VertexBuffer.h"

class VertexArray 
{
private:
	unsigned m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void Bind();
	void Unbind();

	void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout);
};