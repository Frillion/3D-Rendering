#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ASSERT(x) if ((!x)) exit(1)
#define GLCall(x) ClearGLErrors();\
        x;\
        ASSERT(CheckGLErrors(#x, __FILE__, __LINE__));

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

static void ClearGLErrors(){
    while(glGetError());
}

static unsigned int CheckGLErrors(const char* function, const char* file, int line){
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error] " << error << ": " << function << file << line << std::endl;
        return 0;
    }
    return 1;
}

static unsigned int CompileShader(const std::string& source, unsigned int type){
    unsigned int id = glCreateShader(type);
    const char* src = &source[0];
    GLCall(glShaderSource(id, 1, &src, nullptr));;
    GLCall(glCompileShader(id));;

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));;
    if(result == GL_FALSE){
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));;
        char * message = (char *)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));;
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader: " << id << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));;
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vertex_shader = CompileShader(vertexShader,GL_VERTEX_SHADER);
    unsigned int fragment_shader = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLCall(glAttachShader(program, vertex_shader));;
    GLCall(glAttachShader(program, fragment_shader));;
    GLCall(glLinkProgram(program));;
    GLCall(glValidateProgram(program));;

    GLCall(glDeleteShader(vertex_shader));;
    GLCall(glDeleteShader(fragment_shader));;

    return program;
}

int main(void){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cout << "GLEW ERROR" << std::endl;
    }
    
    std::cout << glGetString(GL_VERSION) << std::endl;

    vertex triangle_coordinates[6] = {
        { -0.5f, -0.5f, 0.0f },
        { 0.5f, -0.5f, 0.0f },
        { 0.5f, 0.5f, 0.0f },
        { -0.5f, 0.5f, 0.0f },
    };

    unsigned int indecies[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int triangle_buffer;
    GLCall(glGenBuffers(1, &triangle_buffer));;
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer));;
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * 6, triangle_coordinates, GL_STATIC_DRAW));;
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));;
    GLCall(glEnableVertexAttribArray(0));;


    unsigned int index_buffer;
    GLCall(glGenBuffers(1, &index_buffer));;
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));;
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indecies, GL_STATIC_DRAW));;


    shaderProgramSource source = ParseShader("D:/3D-Rendering/resources/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    GLCall(glUseProgram(shader));;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));;

        GLCall(GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)));;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));;

    glfwTerminate();;
    return 0;
}
