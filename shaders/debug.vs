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
    gl_Position = projection * view * model * vec4(position, 1.0f);
    if(material.texture_normal){
        vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
        vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
        // re-orthogonalize T with respect to N
        T = normalize(T - dot(T, N) * N);
        vec3 B = cross(N, T);
        // TBN must form a right handed coord system.
        // Some models have symetric UVs. Check and fix.
        if (dot(cross(N, T), B) < 0.0)
            T = -T;
        mat3 TBN = transpose(mat3(T, B, N));
        vec3 normal = texture(material.texture_normal1, texCoords).rgb;
        normal = normalize(vec3(model * vec4(normal * 2.0 - 1.0, 1.0)));
        vs_out.normal = normalize(TBN * mat3(view) * normal);
    }
    else
        vs_out.normal = normalize(vec3(vec4(transpose_inverse_viewmodel * normal, 1.0)));
}