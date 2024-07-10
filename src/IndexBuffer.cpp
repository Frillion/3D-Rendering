#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count)
    : Icount(count)
{
    GLCall(glGenBuffers(1, &Renderer_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer(){
    GLCall(glDeleteBuffers(1, &Renderer_id));
}

void IndexBuffer::Bind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer_id));
}

void IndexBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
