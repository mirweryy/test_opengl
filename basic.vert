#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inColor;

out vec4 outColor;
uniform vec3 uniformvec;

void main()
{
    gl_Position = vec4(aPos + uniformvec, 1.0);
    outColor = vec4(inColor, 1.0);
}
