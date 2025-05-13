#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

// int window_height = 1080;
// int window_width = 1920;

// void glfw_window_size_callback(GLFWwindow* window, int width, int height)
// {
//     window_height = height;
//     window_width = width;
//     glViewport(0, 0, window_width, window_height); //до куда мы рисуем в окне
// }

// void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
// {
//     //action -- состояние кнопки "нажата или опущена"
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//     {
//         glfwSetWindowShouldClose(window, GL_TRUE);
//     }
// }

// int main(void)
// {
//     GLFWwindow* window;

//     /* Initialize the library */
//     if (!glfwInit())
//     {
//         std::cout << "Not init glfw" << "\n";
//         return -1;
//     }

//     /* Create a windowed mode window and its OpenGL context */
//     window = glfwCreateWindow(window_width, window_height, "Window", nullptr, nullptr);
//     if (!window)
//     {
//         std::cout << "Not open window" << "\n";
//         glfwTerminate();
//         return -1;
//     }

//     //ивенты окна
//     glfwSetWindowSizeCallback(window, glfw_window_size_callback);
//     glfwSetKeyCallback(window, glfw_key_callback);
//     /* Make the window's context current */
//     glfwMakeContextCurrent(window);

//     if (!gladLoadGL())
//     {
//         std::cout << "Not load gladGL" << "\n";
//         return -1;
//     }

//     std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
//     std::cout << "Versions OpenGL: " << glGetString(GL_VERSION) << "\n";

//     glClearColor(1, 0, 0, 1);

//     /* Loop until the user closes the window */
//     while (!glfwWindowShouldClose(window))
//     {
//         /* Render here */
//         glClear(GL_COLOR_BUFFER_BIT);

//         /* Swap front and back buffers */
//         glfwSwapBuffers(window);

//         /* Poll for and process events */
//         glfwPollEvents();
//     }

//     glfwTerminate();
//     return 0;
// }

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

    glViewport(0, 0, 128, 128);
    glfwSetWindowSizeCallback(window, resize_callback);
    glfwSetKeyCallback(window, key_callback);

    float polygon[9] = 
    {
        -1.0, 0.0, 0.0,
         0.0, 1.0, 0.0,
         1.0, 0.0, 0.0
    };

    unsigned int VBO_polygon;
    glGenBuffers(1, &VBO_polygon);

    unsigned int VAO_polygon;
    glGenVertexArrays(1, &VAO_polygon);
    glBindVertexArray(VAO_polygon);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, polygon, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    //парсин шейдеров
    std::ifstream file("basic.vs");
    std::string out_string = "";
    std::string temp_str = "";
    if (!file)
    {
        std::cout << "Not open file vs" << std::endl;
    }
    while(std::getline(file, temp_str))
    {
        out_string += temp_str;
        out_string += "\n";
    }
    file.close();

    //костыль для перевода строки в массив строк
    GLchar const* files[] = { out_string.c_str()};

    //создание vertex шейдера и его компиляция
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, files, NULL);
    glCompileShader(vertex_shader);

    char resultInfo[1000];
    int res;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &res);
    
    //проверка на ошибки компиляции
    if (!res)
    {
        glGetShaderInfoLog(vertex_shader, 1000, NULL, resultInfo);
        std::cout << "Error compile vertex shader: " << resultInfo;
        glfwTerminate();
        return -1;
    }

    file.open("basic.fs");
    out_string = "";
    if (!file)
    {
        std::cout << "Not open file fs" << std::endl;
    }
    while(std::getline(file, temp_str))
    {
        out_string += temp_str;
        out_string += "\n";
    }
    file.close();

    //костыль для перевода строки в массив строк
    GLchar const* files2[] = { out_string.c_str()};

    //создание fragment шейдера и его компиляция
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, files2, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &res);
    
    //проверка компиляции fragment шейдер
    if (!res)
    {
        glGetShaderInfoLog(fragment_shader, 1000, NULL, resultInfo);
        std::cout << "Error compile fragment shader: " << resultInfo;
        glfwTerminate();
        return -1;
    }

    unsigned int shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vertex_shader);
    glAttachShader(shader_prog, fragment_shader);
    glLinkProgram(shader_prog);

    glGetProgramiv(shader_prog, GL_LINK_STATUS, &res);
    
    //проверка компиляции fragment шейдер
    if (!res)
    {
        glGetProgramInfoLog(shader_prog, 1000, NULL, resultInfo);
        std::cout << "Error linking fragment shader: " << resultInfo;
        glfwTerminate();
        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.9, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT); 

        glUseProgram(shader_prog);
        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}