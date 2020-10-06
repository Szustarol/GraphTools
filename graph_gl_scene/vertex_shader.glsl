#version 330 core
layout (location = 0) in vec3 vertexPosition;

out vec4 vertexColor;

void main(void)
{
    vertexColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gl_Position = vec4(vertexPosition, 1.0f);
}
