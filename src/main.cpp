#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int window_height = 1080;
int window_width = 1920;

void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
    window_height = height;
    window_width = width;
    glViewport(0, 0, window_width, window_height); //до куда мы рисуем в окне
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Not init glfw" << "\n";
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "Window", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Not open window" << "\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, glfw_window_size_callback);
    glfwSetKeyCallback(window, glfw_key_callback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Not load gladGL" << "\n";
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "Versions OpenGL: " << glGetString(GL_VERSION) << "\n";

    glClearColor(1, 0, 0, 1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

