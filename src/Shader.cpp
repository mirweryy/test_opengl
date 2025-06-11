#include "Shader.h"

std::string parsing_text(const std::string path)
{
    std::ifstream file(path);
    std::string out_string = "";
    std::string temp_str = "";
    if (!file)
    {
        std::cout << "Not open file in path" << std::endl;
    }
    while(std::getline(file, temp_str))
    {
        out_string += temp_str;
        out_string += "\n";
    }
    file.close();
    return out_string;
}

unsigned int shader_compiler(std::string path,  GLenum type_shader)
{
    //Чтение файла шейдера
    std::string out_string = parsing_text(path);

    //Перевода строки в массив строк
    GLchar const* file_text[] = { out_string.c_str()};

    //Создание и компиляция шейдера
    unsigned int shader_id = glCreateShader(type_shader);
    glShaderSource(shader_id, 1, file_text, NULL);
    glCompileShader(shader_id);
    return shader_id;
}

Shader::Shader(const std::string vertex_path, const std::string fragment_path)
{
    unsigned int vertex_shader = shader_compiler(vertex_path, GL_VERTEX_SHADER);
    checkCompileErrors(vertex_shader, "VertexShader");

    unsigned int fragment_shader = shader_compiler(fragment_path, GL_FRAGMENT_SHADER);
    checkCompileErrors(fragment_shader, "FragmentShader");

    programID = glCreateProgram();
    glAttachShader(programID, vertex_shader);
    glAttachShader(programID, fragment_shader);
    glLinkProgram(programID);
    checkCompileErrors(programID, "Program");
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}


Shader::~Shader()
{
    glDeleteProgram(programID);
}


unsigned int Shader::getID()
{
    return programID;
}


void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    char resultInfo[1024];
    int res;
    //Проверка компиляции программы или шейдера
    if (type == "Program")
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &res);
        if (!res)
        {
            glGetProgramInfoLog(shader, 1024, NULL, resultInfo);
            std::cout << "Error link shader: " << resultInfo << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
        if (!res)
        {
            glGetShaderInfoLog(shader, 1024, NULL, resultInfo);
            std::cout << "Error link shader: " << resultInfo << std::endl;
        }
    }
}


void Shader::set_float(std::string name, float* values, size_t values_size)
{
    switch(values_size)
    {
        case 1: glUniform1f(glGetUniformLocation(programID, name.c_str()), values[0]); break;
        case 2: glUniform2f(glGetUniformLocation(programID, name.c_str()), values[0], values[1]); break;
        case 3: glUniform3f(glGetUniformLocation(programID, name.c_str()), values[0], values[1], values[2]); break;
        case 4: glUniform4f(glGetUniformLocation(programID, name.c_str()), values[0], values[1], values[2], values[3]); break;
        default:
        std::cout << "Values size in uniform > 4 or < 1" << std::endl;
    }
}


void Shader::use()
{
    glUseProgram(programID);
}