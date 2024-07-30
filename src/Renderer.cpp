#include "Renderer.h"
#include <iostream>

void ClearGLErrors(){
    while(glGetError());
}

unsigned int CheckGLErrors(const char* function, const char* file, int line){
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error] " << error << ": " << function << file << line << std::endl;
        return 0;
    }
    return 1;
}

void Renderer::Draw(const VertexArray& vertex_attrib, const IndexBuffer& index_buffer, const Shader& program) const {
    program.Bind();
    vertex_attrib.Bind();
    index_buffer.Bind(); 
    GLCall(glDrawElements(GL_TRIANGLES, index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr));

}
