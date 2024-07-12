#include "Shader.h"
#include "Renderer.h"
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& file_path):file_path(file_path){
    std::ifstream stream(file_path);
    enum class shaderType{
        None = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    shaderType  type = shaderType::None;
    while (getline(stream, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = shaderType::VERTEX;
            }

            else if(line.find("fragment") != std::string::npos){
                type = shaderType::FRAGMENT;
            }
            continue;
        }
        ss[(int)type] << line << '\n';
    }

    shader_source = { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(const std::string& source, GLenum type){
    unsigned int id = glCreateShader(type);
    const char* src = &source[0];
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if(result == GL_FALSE){
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char * message = (char *)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << source << std::endl;
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader: " << id << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

void Shader::Link(){
    program = glCreateProgram();
    unsigned int vertex_shader = CompileShader(shader_source.vertexSource,  GL_VERTEX_SHADER);
    unsigned int fragment_shader = CompileShader(shader_source.fragmentSource, GL_FRAGMENT_SHADER);

    GLCall(glAttachShader(program, vertex_shader));
    GLCall(glAttachShader(program, fragment_shader));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vertex_shader));
    GLCall(glDeleteShader(fragment_shader));
}

void Shader::Bind(){
        GLCall(glUseProgram(program));
}


void Shader::UnBind(){
    GLCall(glUseProgram(0))
}

Shader::~Shader(){
    GLCall(glDeleteProgram(program));
}
