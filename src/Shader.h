#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

class Shader
{
public:
    Shader(const std::string vertex_path, const std::string fragment_path);
    ~Shader();
    unsigned int getID();
    void set_float(std::string name, float* values, size_t values_size);
    void use();

private:
    unsigned int programID;
    void checkCompileErrors(unsigned int shader, std::string type);
};