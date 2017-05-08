#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec3 Position;
out vec4 FogDist;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vec4 pos = model * vec4(position, 0.0f, 1.0f);
    gl_Position = projection * view * pos;
    Position = pos.xyz;
    TexCoord = texCoord;
    pos.y = 0;
    mat4 static_height_view = mat4(mat3(view));
    FogDist = static_height_view * pos;
}