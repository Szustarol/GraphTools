#version 330 core
layout (location = 0) in vec3 vertexPosition;

out vec4 vertexColor;

uniform mat4 view_matrix;

void main(void)
{
    vertexColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    gl_Position = view_matrix * vec4(vertexPosition, 1.0f);
}
