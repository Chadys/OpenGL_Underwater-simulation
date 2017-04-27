#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 fragcolor;

uniform sampler2D texture_diffuse1;
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
uniform DirLight dirLight;
uniform vec3 viewPos;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 hsv2rgb(vec3 c);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	if(back)
		fragcolor   = vec4(0,0,0,alpha);
    else{
        vec3 color = CalcDirLight(dirLight, norm, viewDir);
    	fragcolor   = vec4(hsv2rgb(modelColor), alpha) * vec4(color, 1.0);
    }
	if(fragcolor.a<0.1)
		discard;
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
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}