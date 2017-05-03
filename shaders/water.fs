#version 330 core
in vec2 TexCoord;
in vec3 Position;
in mat3 TBN;
in vec4 EyeSpacePos;

out vec4 fragcolor;

uniform sampler2D normals;
uniform samplerCube skybox;

uniform struct FogParameters
{
   vec4 vFogColor; // Fog color
   float fStart; // This is only for linear fog
   float fEnd; // This is only for linear fog
   float fDensity; // For exp and exp2 equation

   int iEquation; // 0 = linear, 1 = exp, 2 = exp2
} fogParams;

uniform float fade;
uniform vec3 viewPos;

float getFogFactor(FogParameters params, float fFogCoord);

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

    // Add fog
    float fFogCoord = abs(EyeSpacePos.z);
    fragcolor = mix(fragcolor, fogParams.vFogColor, getFogFactor(fogParams, fFogCoord));
}