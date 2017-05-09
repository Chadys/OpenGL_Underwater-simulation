#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D screenTexture;
uniform int effect;

vec4 get_fisheye();

const float PI = 3.1415926535;

void main(){
    fragcolor = get_fisheye();
}

//http://www.geeks3d.com/20140213/glsl-shader-library-fish-eye-and-dome-and-barrel-distortion-post-processing-filters/
vec4 get_fisheye(){
    float aperture = 200.0;
    float apertureHalf = 0.5 * aperture * (PI / 180.0);
    float maxFactor = sin(apertureHalf);

    vec2 uv;
    vec2 xy = 2.0 * TexCoords - 1.0;
    float d = length(xy);
    if (d < (2.0-maxFactor)){
        d = length(xy * maxFactor);
        float z = sqrt(1.0 - d * d);
        float r = atan(d, z) / PI;
        float phi = atan(xy.y, xy.x);

        uv.x = r * cos(phi) + 0.5;
        uv.y = r * sin(phi) + 0.5;
    }
    else{
      uv = TexCoords;
    }
    return texture(screenTexture, uv);
}