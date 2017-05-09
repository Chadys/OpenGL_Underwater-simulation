#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D screenTexture;
uniform int effect;

vec4 get_kalei2();
void main(){

    fragcolor = get_kalei2();
}

//http://stackoverflow.com/questions/13739901/vertex-kaleidoscope-shader
vec4 get_kalei2(){
    float sides = 10;
    float offset = 0.3;

    vec2 p = TexCoords - 0.5;
    float r = length(p);
    float a = atan(p.y, p.x);
    float tau = 2.0 * 3.1416;
    a = mod(a, tau/sides);
    a = abs(a - tau/sides/2.0);
    vec2 position = r * vec2(cos(a), sin(a)) + 0.5  + offset;
    return texture(screenTexture, position);
}