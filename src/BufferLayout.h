#pragma once
#include "macros.h"
#include "VertexBuffer.h"
#include <vector>
#include <GL/glew.h>

struct BufferElement{
	unsigned int count;
	unsigned int type;
	unsigned int normalized;
	static unsigned int GetSizeOfType(unsigned int type){
		switch(type){
			case GL_FLOAT:		return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false)	
	}
};

class BufferLayout{
	private:
		std::vector<BufferElement> buffer_elements;
		unsigned int Stride;
	public:
		BufferLayout():
			Stride(0){}

		template<typename T>
		void Push(unsigned int count){
			static_assert(false);
		}

		inline const std::vector<BufferElement>GetElements() const&{ return buffer_elements; }
		inline unsigned int GetStride()const { return Stride; }
};


template<>
inline void BufferLayout::Push<float>(unsigned int count){
    buffer_elements.push_back({count, GL_FLOAT, GL_FALSE});
    Stride += BufferElement::GetSizeOfType(GL_FLOAT)*count;
}

template<>
inline void BufferLayout::Push<unsigned int>(unsigned int count){
    buffer_elements.push_back({count,GL_UNSIGNED_INT, GL_FALSE});
    Stride += BufferElement::GetSizeOfType(GL_UNSIGNED_INT)*count;
}

template<>
inline void BufferLayout::Push<unsigned char>(unsigned int count){
    buffer_elements.push_back({count,GL_UNSIGNED_BYTE, GL_TRUE});
    Stride += BufferElement::GetSizeOfType(GL_UNSIGNED_INT)*count;
}

