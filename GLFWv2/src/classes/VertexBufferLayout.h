#pragma once
#include <vector>
#include <GL\glew.h>
#include "ErrorHandler.h"

struct VertexBufferElement
{
	unsigned count;
	unsigned type;
	unsigned char normalized;

	static unsigned GetSizeOfType(unsigned type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned m_Stride;
public:
	VertexBufferLayout();

	template<typename T> void Push(unsigned count);
	template<> void Push<float>(unsigned count);
	template<> void Push<unsigned>(unsigned count);
	template<> void Push<unsigned char>(unsigned count);

	unsigned GetStride() const;
	std::vector<VertexBufferElement> GetElements() const;
};

template<typename T>
inline void VertexBufferLayout::Push(unsigned count)
{
	static_assert(false);
}

template<>
inline void VertexBufferLayout::Push<float>(unsigned count)
{
	m_Elements.push_back({ count, GL_FLOAT, GL_FALSE });
	m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
inline void VertexBufferLayout::Push<unsigned>(unsigned count)
{
	m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_TRUE });
	m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned count)
{
	m_Elements.push_back({count,GL_UNSIGNED_BYTE, GL_TRUE});
	m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}