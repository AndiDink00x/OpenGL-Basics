#include "VertexBuffer.h"
#include "Renderer.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	//Generate Vertex Buffer
	GLCall(glGenBuffers(1, &m_RendererID));
	//Bind Vertex Buffer to the ID reference
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	//Introduce vertices to the Vertex Buffer
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	//Delete V.Buffer Created
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
