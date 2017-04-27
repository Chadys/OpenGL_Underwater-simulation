#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 deplace;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoords =  texCoord+deplace;
}