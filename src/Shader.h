#pragma once
#include <GL/glew.h>
#include <unordered_map>
#include <string>
struct shaderProgramSource{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader{
    private:
	unsigned int Renderer_id;
	const std::string& file_path;
	std::unordered_map<std::string, int> UniformLocationCahce;

    public:
        Shader(const std::string& filebuff);
        shaderProgramSource ParseShader(const std::string& filepath);
        unsigned int CreateShader(std::string&vertexSource, std::string&fragmentSource);
        unsigned int CompileShader(const std::string& source, GLenum type);
        void SetUniform1i(const std::string& name, int v0);
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        int GetUniformLocation(const std::string& name);
        void Bind() const;
        void UnBind() const;
        ~Shader();
};
