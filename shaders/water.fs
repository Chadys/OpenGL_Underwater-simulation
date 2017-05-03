#version 330 core
in vec2 TexCoord;
in vec3 Position;
in mat3 TBN;

out vec4 fragcolor;

uniform sampler2D normals;
uniform samplerCube skybox;

uniform float fade;
uniform vec3 viewPos;

void main()
{
	vec4 color;
	vec3 normal = texture(normals, TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    //normal = normalize(TBN * normal);

    vec3 I = TBN * normalize(Position - viewPos);
    vec3 R = reflect(I, normal);
    color = texture(skybox, R);
    fragcolor = vec4((normal+1)/2,1);
}