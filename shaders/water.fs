#version 330 core
in vec3 Position;
in vec4 FogDist;
in vec2 TexCoord;

out vec4 fragcolor;

uniform samplerCube skybox;

uniform struct FogParameters{
   vec4 vFogColor; // Fog color
   float fStart; // This is only for linear fog
   float fEnd; // This is only for linear fog
   float fDensity; // For exp and exp2 equation

   int iEquation; // 0 = linear, 1 = exp, 2 = exp2
} fogParams;
uniform float fade;
uniform vec3 viewPos;
uniform float time;


const float pi = 3.14159;
const float wavelength = 50*pi;
const float speed = 10.0;
const vec2 direction = vec2(pi/2, pi);

vec3 waveNormal(vec3 pos) {
    float frequency = 2*pi/wavelength;
    float phase = speed * frequency;
    float theta = dot(direction, pos.xz);
    float amplitude = 0.5 * direction.x * frequency;
    float red = amplitude * cos(direction.x * pos.x * frequency + time * phase);
    amplitude = amplitude * direction.y * frequency;
    float green = amplitude * cos(direction.y * pos.z * frequency + time * phase);
    vec3 n = vec3(red,1+abs(red)+abs(green),green);

    /*float red = 0.8*sin(0.07*pos.x + time);
    float green = 0.8*cos(0.05*pos.z + time);
    vec3 n = vec3(red,1+abs(red)+abs(green),green);*/
    return normalize(n);
}


float getFogFactor(FogParameters params, float fFogCoord);

void main()
{
	vec3 Normal = waveNormal(Position);
    float ratio = 1.00 / 1.33; //water refractive index

    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, Normal);
    vec4 mirror_color = texture(skybox, R);
    R = refract(I, Normal, ratio);
    vec4 distord_color = texture(skybox, R);
    // Mix refraction and reflection
    fragcolor = mix(mirror_color, distord_color, 0.8);
    //Add water color
    fragcolor = mix(vec4(0.037, 0.17, 0.33, 1), distord_color, 0.5);

    // Add fog
    float fFogCoord = abs(FogDist.z);
    fragcolor.a = max(0.9 - getFogFactor(fogParams, fFogCoord), 0.0);

    fragcolor = fragcolor*fade;
    if (fragcolor.a < 0.0001)
        discard;
}