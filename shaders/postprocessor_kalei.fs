#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D screenTexture;
uniform int effect;

uniform float width;
uniform float height;

vec4 get_kalei();


void main(){

    fragcolor = get_kalei();
}

//https://github.com/dff180/pishadertoy/blob/master/shaders/kaleidoscope.f.glsl
vec4 get_kalei(){

    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / vec2(width, height);
    vec2 uv;

    float a = atan(p.y,p.x);
    float r = sqrt(dot(p,p));

    uv.x =          7.0*a/3.1416;
    uv.y = -sin(7.0*r) + .7*cos(7.0*a);

    float w = .5+.5*(sin(7.0*r)+.7*cos(7.0*a));

    vec4 col = texture(screenTexture,uv*.5);
    return vec4(col.xyz*w, col.a);
}