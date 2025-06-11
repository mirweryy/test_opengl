#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>

void resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << width << "x" << height << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //action -- состояние кнопки "нажата или опущена"
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(256, 256, "Window", NULL, NULL);
    if (!window)
    {
        std::cout << "sorry, not create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "not Load GL" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << GLVersion.major << "."<< GLVersion.minor << std::endl;

    glViewport(0, 0, 256, 256);
    glfwSetWindowSizeCallback(window, resize_callback);
    glfwSetKeyCallback(window, key_callback);

    const int dot = 6;

    float polygon[dot * 4] = 
    {
        -0.5, 0.5, 0.0,   1.0, 0.0, 0.0,
        -0.5,-0.5, 0.0,   1.0, 1.0, 0.0,
         0.5,-0.5, 0.0,   1.0, 0.0, 0.0,
         0.5, 0.5, 0.0,   1.0, 0.0, 0.0,
    };


    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VBO_polygon, VAO_polygon, EBO_polygon;

    glGenBuffers(1, &VBO_polygon);
    glGenVertexArrays(1, &VAO_polygon);
    glGenBuffers(1, &EBO_polygon);


    glBindVertexArray(VAO_polygon);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygon);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dot * 4, polygon, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader* test_shader = new Shader("basic.vert", "basic.frag");
    float position[3] = {0.0f, 0.0f, 0.0f};
    int temp = 0;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.9, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT); 

        position[0] = cos(glfwGetTime());
        position[1] = cos(glfwGetTime());

        test_shader->use();
        test_shader->set_float("uniformvec", position, 3);

        glBindVertexArray(VAO_polygon);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}