#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int m_Count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};