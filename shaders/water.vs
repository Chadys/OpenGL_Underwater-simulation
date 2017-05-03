#version 330 core
layout (location = 0) in vec2 position;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;
out vec3 Position;
out mat3 TBN;
out vec4 EyeSpacePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 tangent = vec3(1,0,0);
    vec3 normal = vec3(0,0,1);
    vec4 pos = vec4(position, 0.0f, 1.0f);
    EyeSpacePos = view * model * pos;
    gl_Position = projection * EyeSpacePos;
    TexCoord =  texCoord;
    Position = (model * pos).xyz;
    vec3 T = normalize((model * vec4(tangent,   0.0)).xyz);
    vec3 N = normalize((model * vec4(normal,    0.0)).xyz);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
}