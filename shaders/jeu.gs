#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 TexCoords[];
out vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform bool isBillboard;
uniform vec2 Billboard_Size;
uniform vec3 cam_up;
uniform vec3 cam_right;


void main()
{
    for(int i = 0; i < gl_in.length(); i++){
      gl_Position = gl_in[i].gl_Position;
      TexCoord = TexCoords[i];
      EmitVertex();
    }
    EndPrimitive();
    return;
}
