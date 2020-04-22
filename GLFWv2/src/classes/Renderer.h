#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

class Renderer
{
public:
	void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader);
	void Clear();
};