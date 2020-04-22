#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout() : m_Stride(0)
{
}

unsigned VertexBufferLayout::GetStride() const
{
	return m_Stride;
}

std::vector<VertexBufferElement> VertexBufferLayout::GetElements() const
{
	return m_Elements;
}
