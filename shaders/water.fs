#version 330 core
in vec2 TexCoord;
in vec3 Position;

out vec4 fragcolor;

uniform sampler2D normals;
uniform samplerCube skybox;

uniform float fade;
uniform vec3 viewPos;

void main()
{
	vec4 color;
	vec3 normal = texture(normals, TexCoord).rgb;
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(normal));
    color = texture(skybox, R);
    fragcolor = vec4(normal,1);
}