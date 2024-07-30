#include "Shader.h"
#include "Renderer.h"
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& file_path):
	file_path(file_path), Renderer_id(0){
	shaderProgramSource source = ParseShader(file_path);
	Renderer_id = CreateShader(source.vertexSource, source.fragmentSource);
}

shaderProgramSource Shader::ParseShader(const std::string& file_path){
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

    return { ss[0].str(), ss[1].str() };
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

unsigned int Shader::CreateShader(std::string& vertexSource, std::string&fragmentSource){
	unsigned int program = glCreateProgram();
	unsigned int vertex_shader = CompileShader(vertexSource,  GL_VERTEX_SHADER);
	unsigned int fragment_shader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(program, vertex_shader));
	GLCall(glAttachShader(program, fragment_shader));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vertex_shader));
	GLCall(glDeleteShader(fragment_shader));

	return program;
}

void Shader::Bind() const{
        GLCall(glUseProgram(Renderer_id));
}


void Shader::UnBind() const{
    GLCall(glUseProgram(0))
}

Shader::~Shader(){
    GLCall(glDeleteProgram(Renderer_id));
}
