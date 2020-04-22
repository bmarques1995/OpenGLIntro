#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1,&m_RendererID));
}

void VertexArray::Bind()
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind()
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout)
{
	Bind();
	vertexBuffer.Bind();
	const auto& elements = vertexBufferLayout.GetElements();
	unsigned offset = 0;
	for  (unsigned i = 0; i < elements.size(); i++)
	{
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i,	elements[i].count, elements[i].type, elements[i].normalized, vertexBufferLayout.GetStride(), (const void *)offset));
		offset += elements[i].count;

	}
	
}
