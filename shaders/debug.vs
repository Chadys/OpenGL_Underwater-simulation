#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoords;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat3 transpose_inverse_viewmodel;
uniform mat4 view;
uniform mat4 projection;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    bool texture_specular;
    sampler2D texture_emissive1;
    bool texture_emissive;
    sampler2D texture_height1;
    bool texture_height;
    sampler2D texture_normal1;
    bool texture_normal;
    sampler2D texture_opacity1;
    bool texture_opacity;
    float shininess;
};
uniform Material material;

void main()
{
    vec3 final_normal = texture(material.texture_normal1, texCoords).rgb;
    gl_Position = projection * view * model * vec4(position, 1.0f);
    if(material.texture_normal){
        vec3 T = normalize(transpose_inverse_viewmodel * tangent);
        vec3 N = normalize(transpose_inverse_viewmodel * normal);
        vec3 B = cross(N, T);
        mat3 TBN = mat3(T, B, N);
        final_normal = normalize(final_normal * 2.0 - 1.0);
        final_normal = normalize(vec4(final_normal, 1.0).xyz);
        vs_out.normal = normalize(TBN * final_normal);
    }
    else{
        final_normal = normal;
        vs_out.normal = normalize(vec3(vec4(transpose_inverse_viewmodel * final_normal, 1.0)));
    }
}