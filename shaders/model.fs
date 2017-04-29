#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

out vec4 fragcolor;

uniform float shininess;
uniform bool back;
uniform float alpha;
uniform vec3 modelColor;

uniform float fade;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    bool texture_specular;
    sampler2D texture_height1;
    bool texture_height;
    sampler2D texture_normal1;
    bool texture_normal;
    sampler2D texture_opacity1;
    bool texture_opacity;
    float shininess;
};
uniform DirLight dirLight;
uniform Material material;
uniform vec3 viewPos;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 hsv2rgb(vec3 c);

void main(){
    float alpha = alpha;
    if (material.texture_opacity)
        alpha = min(alpha, texture(material.texture_opacity1, TexCoords).r); // since opacity texture is a B&W image, we can take r, g or b
	if(alpha<0.1)
		discard;
    vec3 norm;
    if(material.texture_normal){
        norm = texture(material.texture_normal1, TexCoords).rgb;
        norm = normalize(norm);
        norm = normalize(TBN * norm);
    }
    else
        norm = Normal;
    vec3 viewDir = normalize(viewPos - FragPos);
	if(back)
		fragcolor   = vec4(0,0,0,alpha);
    else
    	fragcolor   = vec4(hsv2rgb(modelColor), alpha) * vec4(CalcDirLight(dirLight, norm, viewDir), alpha);
    if(!gl_FrontFacing)
        fragcolor.xyz *= vec3(0.3);
	fragcolor *= fade;
}

//http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec;
    if(material.texture_specular)
        specular*=vec3(texture(material.texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}