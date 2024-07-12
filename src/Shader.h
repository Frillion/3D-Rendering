#pragma once

#include "Renderer.h"
#include <string>

struct shaderProgramSource{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader{
    private:
        const std::string& file_path;
        shaderProgramSource shader_source;
        unsigned int vertex;
        unsigned int fragment;
        unsigned int program;
        unsigned int CompileShader(const std::string& source, GLenum type);

    public:
        Shader(const std::string& filebuff);
        inline unsigned int GetProgram() {return program;}
        void Link();
        void Bind();
        void UnBind();
        ~Shader();
};
