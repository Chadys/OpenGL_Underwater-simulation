#version 330 core
out vec4 color;
in vec3 Normal;

void main()
{
    color = vec4(Normal, 1.0f);
}