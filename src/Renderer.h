#pragma once
#include <iostream>
#include <GL/glew.h>
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

class Renderer{
	public:
        Renderer();
        ~Renderer();
        void Clear() const;
		void Draw(const VertexArray& vertex_attrib, const IndexBuffer& index_buffer, const Shader& program) const;
};
