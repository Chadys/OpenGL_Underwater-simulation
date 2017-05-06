#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform vec2 Billboard_Size;
uniform vec3 cam_up;
uniform vec3 cam_right;


void main(){
    vec3 pos = gl_in[0].gl_Position.xyz;
    gl_Position = projection * view * vec4(pos + cam_right * -0.5 * Billboard_Size.x + cam_up * -0.5 * Billboard_Size.y, 1.0);
    TexCoord = vec2(0,1);
    EmitVertex();
    gl_Position = projection * view * vec4(pos + cam_right * 0.5 * Billboard_Size.x + cam_up * -0.5 * Billboard_Size.y, 1.0);
    TexCoord = vec2(1,1);
    EmitVertex();
    gl_Position = projection * view * vec4(pos + cam_right * -0.5 * Billboard_Size.x + cam_up * 0.5 * Billboard_Size.y, 1.0);
    TexCoord = vec2(0,0);
    EmitVertex();
    gl_Position = projection * view * vec4(pos + cam_right * 0.5 * Billboard_Size.x + cam_up * 0.5 * Billboard_Size.y, 1.0);
    TexCoord = vec2(1,0);
    EmitVertex();
    EndPrimitive();
}
