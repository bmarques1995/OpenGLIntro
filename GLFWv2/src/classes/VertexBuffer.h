#pragma once


class VertexBuffer 
{
private:
	unsigned m_RendererID;
public:
	VertexBuffer(const void *data, unsigned size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};