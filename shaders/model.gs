#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 all_TexCoord[];
out vec2 TexCoord;
in vec3 all_FragPos[];
out vec3 FragPos;
in vec3 all_Normal[];
out vec3 Normal;
in mat3 all_TBN[];
out mat3 TBN;

uniform float time;


vec4 explode(vec4 position, vec3 normal);
vec3 GetSideNormal();

void main(){
    vec3 normal = GetSideNormal();

    for(int i = 0; i < gl_in.length(); i++){
      gl_Position = explode(gl_in[i].gl_Position, normal);
      TexCoord = all_TexCoord[i];
      FragPos = all_FragPos[i];
      Normal = all_Normal[i];
      TBN = all_TBN[i];
      EmitVertex();
    }
    EndPrimitive();
    return;
}

vec4 explode(vec4 position, vec3 normal){
    float magnitude = 1.0f;
    vec3 direction = normal * sin(position.z*3.0+time) * magnitude;
    if (normal.x > 0.0)
        direction = -direction;
    return position + vec4(direction, 0.0f);
}

vec3 GetSideNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 norm = normalize(cross(a, b));
   if (norm.x < 0.1 && norm.x > -0.1)
    norm.x = 0.5;
   return vec3(norm.x, 0, 0);
}