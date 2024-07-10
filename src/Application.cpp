#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


struct vertex{
    float pos[3];
};

vertex new_vertex = {
    {0.5f,-0.5f,0.0f}
};


struct shaderProgramSource{
    std::string vertexSource;
    std::string fragmentSource;
};


static shaderProgramSource ParseShader(const std::string& filebuff){
    std::ifstream stream(filebuff);
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


static unsigned int CompileShader(const std::string& source, unsigned int type){
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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vertex_shader = CompileShader(vertexShader,GL_VERTEX_SHADER);
    unsigned int fragment_shader = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLCall(glAttachShader(program, vertex_shader));
    GLCall(glAttachShader(program, fragment_shader));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vertex_shader));
    GLCall(glDeleteShader(fragment_shader));

    return program;
}

int main(void){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "3D-Rendering", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    GLenum err = glewInit();
    if(err != GLEW_OK){
        std::cout << "GLEW ERROR:" << err << std::endl;
    }
    
    std::cout << glGetString(GL_VERSION) << std::endl;

    vertex coordinates[6] = {
        { { -0.5f, -0.5f, 0.0f } },
        { { 0.5f, -0.5f, 0.0f } },
        { { 0.5f, 0.5f, 0.0f } },
        { { -0.5f, 0.5f, 0.0f } },
    };

    unsigned int indecies[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vertex_attrib;
    GLCall(glGenVertexArrays(1, &vertex_attrib));
    GLCall(glBindVertexArray(vertex_attrib));


    VertexBuffer* vertex_buffer = new VertexBuffer(coordinates, 4 * sizeof(vertex));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));


    IndexBuffer* index_buffer = new IndexBuffer(indecies, 6);

    shaderProgramSource source = ParseShader("/home/Frillion/3D-Rendering/resources/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(int location  = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);

    float red_channel = 0.0f;
    float increment = 0.05f;

    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, red_channel, 0.3f, 0.8f, 1.0f));

        GLCall(glBindVertexArray(vertex_attrib));
        index_buffer->Bind(); 

        GLCall(glDrawElements(GL_TRIANGLES, index_buffer->GetCount(), GL_UNSIGNED_INT, nullptr));

        if (red_channel > 1.0f || red_channel < 0.0f){
            increment = -increment;
        } 
        red_channel += increment; 
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    delete vertex_buffer;
    delete index_buffer;
    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}
