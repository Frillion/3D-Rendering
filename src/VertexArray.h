#pragma once
#include "VertexBuffer.h"
#include "BufferLayout.h"

class VertexArray{
	private:
		unsigned int Renderer_id;
	public:
		VertexArray();
		~VertexArray();
		void Bind() const;
		void Unbind() const;
		void AddBuffer(const VertexBuffer& buffer, const BufferLayout& layout);
};
