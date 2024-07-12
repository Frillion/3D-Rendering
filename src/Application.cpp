#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"


struct vertex{
    float pos[3];
};

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

    Shader* shader = new Shader("/home/Frillion/3D-Rendering/resources/shaders/Basic.shader");
    shader->Link();
    shader->Bind();

    GLCall(int location  = glGetUniformLocation(shader->GetProgram(), "u_Color"));
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
        
        shader->Bind();
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
    delete shader;

    glfwTerminate();
    return 0;
}
