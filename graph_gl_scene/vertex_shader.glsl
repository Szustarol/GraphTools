#version 330 core
layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec4 vertexCol;

out vec4 vertexColor;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;


void main(void)
{
    vertexColor = vertexCol;
    gl_Position =  projection_matrix * view_matrix * model_matrix * vec4(vertexPosition, 1.0f, 1.0f);
}
