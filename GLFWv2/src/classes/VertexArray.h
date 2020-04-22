#pragma once


#include "VertexBufferLayout.h"

#include "VertexBuffer.h"

class VertexArray 
{
private:
	unsigned m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout);
};