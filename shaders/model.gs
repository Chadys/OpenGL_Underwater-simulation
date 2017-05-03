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
in vec3 all_Pos[];
in vec4 all_EyeSpacePos[];
out vec4 EyeSpacePos;

uniform float time;
uniform bool wings;
uniform float magnitude;


vec4 ondulate(vec4 transformed_position, vec3 normal, vec3 position);
vec4 flap(vec4 transformed_position, vec3 normal, vec3 position);
vec3 GetSideNormal();
vec3 GetHeightNormal();

void main(){
    vec3 normal;
    if (wings)
        normal = GetHeightNormal();
    else
        normal = GetSideNormal();

    for(int i = 0; i < gl_in.length(); i++){
        if (wings)
            gl_Position = flap(gl_in[i].gl_Position, normal, all_Pos[i]);
        else
            gl_Position = ondulate(gl_in[i].gl_Position, normal, all_Pos[i]);
        TexCoord = all_TexCoord[i];
        FragPos = all_FragPos[i];
        Normal = all_Normal[i];
        TBN = all_TBN[i];
        EyeSpacePos = all_EyeSpacePos[i];
        EmitVertex();
    }
    EndPrimitive();
    return;
}

vec4 ondulate(vec4 transformed_position, vec3 normal, vec3 position){
    float period = 3.0;
    vec3 direction = normal * sin(period*transformed_position.z+time) * magnitude;
    if (normal.x > 0.0)
        direction = -direction;
    return transformed_position + vec4(direction, 0.0f);
}

vec4 flap(vec4 transformed_position, vec3 normal, vec3 position){
    float period_time = 5.0, period_pos = 0.02;
    vec3 direction = normal * sin(period_pos*position.z+period_time*time) * magnitude * position.x;
    if (normal.y > 0.0)
        direction = -direction;
    if (position.x < 0.0)
        direction = -direction;
    return transformed_position + vec4(direction, 0.0f);
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

vec3 GetHeightNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 norm = normalize(cross(a, b));
   if (norm.y < 0.1 && norm.y > -0.1)
    norm.y = 0.5;
   return vec3(0, norm.y, 0);
}
