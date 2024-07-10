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
