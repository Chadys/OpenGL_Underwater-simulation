#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D screenTexture;
uniform int effect;

uniform float width;
uniform float height;

vec4 get_swirl();

void main(){
    fragcolor = get_swirl();
}

//http://www.geeks3d.com/20110428/shader-library-swirl-post-processing-filter-in-glsl/
vec4 get_swirl(){
    float radius = width/3;
    float angle = 0.8;
    vec2 center = vec2(width/2, height/2);

    vec2 texSize = vec2(width, height);
    vec2 tc = TexCoords * texSize;
    tc-=center;
    float dist = length(tc);
    if (dist < radius){
        float percent = (radius - dist) / radius;
        float theta = percent * percent * angle * 8.0;
        float s = sin(theta);
        float c = cos(theta);
        tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
    }
    tc += center;

    return texture(screenTexture, tc/texSize);
}