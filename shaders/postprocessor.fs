#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D screenTexture;
uniform int effect;


vec4 get_bit_reduce(float n_bits, vec4 color);
vec4 get_halftone(vec4 color);

float rand(vec2 co){
    return fract(sin(dot(co ,vec2(12.9898,78.233))) * 43758.5453);
}

const float N_BIT = 2;

void main(){
    vec4 color = texture(screenTexture, TexCoords);
    switch(effect){
        case 0: // NO_POSTPROD
            fragcolor = color;
            break;
        case 6: //GREYSCALE
            float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
            fragcolor = vec4(average, average, average, 1.0);
            break;
        case 7: //INVERT
            fragcolor = vec4(1-color.rgb, color.a);
            break;
        case 8: //BRIGHT
            fragcolor = vec4(color.rgb * 4, color.a);
            break;
        case 9: //DARK
            fragcolor = vec4(color.rgb * 0.3, color.a);
            break;
        case 10: //THRESHOLD
            if((color.r+color.g+color.b)/3 > 0.4)
                fragcolor = vec4(1);
            else
                fragcolor = vec4(0,0,0,1);
            break;
        case 11: //RED_CANAL
            fragcolor = vec4(color.r,0,0,color.a);
            break;
        case 12: //GREEN_CANAL
            fragcolor = vec4(0,color.g,0,color.a);
            break;
        case 13: //BLUE_CANAL
            fragcolor = vec4(0,0,color.b,color.a);
            break;
        case 14: //BIT_REDUCE
            fragcolor = get_bit_reduce(N_BIT, color);
            break;
        case 18: //HALFTONING
            fragcolor = get_halftone(color);
            break;
        default:
            fragcolor = color;
    }
}

vec4 get_bit_reduce(float n_bits, vec4 color){
    float gray_lvl = pow(2, n_bits);
    float step = 1/gray_lvl;
    for(int i = 0; i<3; i++){
        float col = 0.0;
        while(color[i]-col > step/2)
            col += step;
        color[i] = col;
    }
    return color;
}

vec4 get_halftone(vec4 color){
    float r = rand(TexCoords);
    for(int i = 0; i<3; i++){
        if(r < color[i])
            color[i] = 1;
        else
            color[i] = 0;
    }
    return color;
}