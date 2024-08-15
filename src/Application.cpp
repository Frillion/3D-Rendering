#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


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
    int width,height;
    glfwGetWindowSize(window, &width, &height);
    float fwidth = (float)width;
    float fheight = (float)height;
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

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    float coordinates[] = {
          100.0f, 100.0f, 0.0f, 0.0f,
          200.0f, 100.0f, 1.0f, 0.0f,
          200.0f, 200.0f, 1.0f, 1.0f,
          100.0f, 200.0f, 0.0f, 1.0f,
    };

    unsigned int indecies[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    VertexArray* vertex_attrib = new VertexArray();
    VertexBuffer* vertex_buffer = new VertexBuffer(coordinates, 16 * sizeof(float));
    BufferLayout* layout = new BufferLayout();
    layout->Push<float>(2);
    layout->Push<float>(2);
    vertex_attrib->AddBuffer(*vertex_buffer, *layout);

    IndexBuffer* index_buffer = new IndexBuffer(indecies, 6);
    
    glm::mat4 proj = glm::ortho(0.0f, fwidth, 0.0f, fheight, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
    glm::mat4 mvp =  proj * view * model;

    Shader* shader = new Shader("/home/Frillion/Projects/personal/3D-Rendering/resources/shaders/Basic.shader");
    shader->Bind();

    Texture* texture = new Texture("/home/Frillion/Projects/personal/3D-Rendering/resources/kirby.png");
    texture->Bind();
    shader->SetUniform1i("u_Texture", 0);
    shader->SetUniformMat4f("u_MVP", mvp);

    float red_channel = 0.0f;
    float increment = 0.05f;

    vertex_attrib->Unbind();
    shader->UnBind();
    vertex_buffer->Unbind();
    index_buffer->Unbind();

    Renderer renderer;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        renderer.Clear();        

        shader->Bind();
        // shader->SetUniform4f("u_Color",red_channel, 0.0f, 0.0f, 1.0f);

        renderer.Draw(*vertex_attrib, *index_buffer, *shader);

        if (red_channel > 1.0f || red_channel < 0.0f){
            increment = -increment;
        } 
        red_channel += increment; 
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    delete vertex_attrib;
    delete layout;
    delete vertex_buffer;
    delete index_buffer;
    delete shader;
    delete texture;

    glfwTerminate();
    return 0;
}
