#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

out vec4 fragcolor;

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
uniform DirLight dirLight;
uniform Material material;
uniform vec3 viewPos;
uniform mat3 transpose_inverse_viewmodel;

// Function prototypes
vec3 CalcDirLight(DirLight light, Material mat, vec3 normal, vec3 viewDir, vec4 textures[6]);
vec3 hsv2rgb(vec3 c);
vec4[6] get_textures(Material mat, vec2 coord);

void main(){
    vec4 textures[6] = get_textures(material, TexCoord);
    float alpha = alpha;
    if (material.texture_opacity){
        alpha = min(alpha, textures[5].r); // since opacity texture is a B&W image, we can take r, g or b
	}
	if(alpha<0.1){
		discard;
	}
    vec3 norm;
    if(material.texture_normal){
        norm = textures[4].rgb;
        norm = normalize(norm * 2.0 - 1.0);
        norm = normalize(vec4(norm, 1.0).xyz);
        norm = normalize(TBN * norm);
    }
    else
        norm = Normal;
    vec3 viewDir = normalize(viewPos - FragPos);
	if(back){
		fragcolor   = vec4(0,0,0,alpha);
	}
    else{
        vec3 true_colors = CalcDirLight(dirLight, material, norm, viewDir, textures);
        if(material.texture_emissive){
            vec3 emission = textures[2].rgb;
            true_colors += emission;
        }
    	fragcolor   = vec4(hsv2rgb(modelColor), alpha) * vec4(true_colors, 1.0);
    }
	fragcolor *= fade;
}

//http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 hsv2rgb(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4[6] get_textures(Material mat, vec2 coord){
    vec4 textures[6];
    textures[0] = texture(mat.texture_diffuse1, coord);
    textures[1] = texture(mat.texture_specular1, coord);
    textures[2] = texture(mat.texture_emissive1, coord);
    textures[3] = texture(mat.texture_height1, coord);
    textures[4] = texture(mat.texture_normal1, coord);
    textures[5] = texture(mat.texture_opacity1, coord);
    return textures;
}