#include "VertexArray.h"
#include "BufferLayout.h"

VertexArray::VertexArray(){
	GLCall(glGenVertexArrays(1,&Renderer_id));
}
VertexArray::~VertexArray(){
	GLCall(glDeleteVertexArrays(1,&Renderer_id));
}

void VertexArray::Bind() const{
	GLCall(glBindVertexArray(Renderer_id));
}

void VertexArray::Unbind() const{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const BufferLayout& layout){
	Bind();
	buffer.Bind();
	const std::vector<BufferElement> elements = layout.GetElements();
	unsigned int offset = 0;
	for(int i = 0; i < elements.size(); i++){
		BufferElement element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
	    	GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,\
				   layout.GetStride(), (const void*)offset));
		offset += element.count * BufferElement::GetSizeOfType(element.type);
	}
}
