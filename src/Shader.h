#pragma once

#include "Renderer.h"
#include <string>

struct shaderProgramSource{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader{
    private:
	unsigned int Renderer_id;
	const std::string& file_path;

    public:
        Shader(const std::string& filebuff);
	shaderProgramSource ParseShader(const std::string& filepath);
        unsigned int CreateShader(std::string&vertexSource, std::string&fragmentSource);
        unsigned int CompileShader(const std::string& source, GLenum type);
        void Bind() const;
        void UnBind() const;
        ~Shader();
};
