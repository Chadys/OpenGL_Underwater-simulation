#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 TexCoords[];
out vec2 TexCoord;
in vec3 Positions[];
out vec3 Position;


void main()
{
    for(int i = 0; i < gl_in.length(); i++){
      gl_Position = gl_in[i].gl_Position;
      TexCoord = TexCoords[i];
      Position = Positions[i];
      EmitVertex();
    }
    EndPrimitive();
    return;
}
