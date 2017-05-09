#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D screenTexture;
uniform int effect;

uniform vec2 kernel[9];
uniform float average_kernel[9];
uniform float sharpen_kernel[9];
uniform float gauss_kernel[9];
uniform float sobel_kernel[9];
uniform float laplacian_kernel[9];

vec4 apply_filter(float filter_kernel[9], vec4 K[9]);
vec4 get_bit_reduce(float n_bits, vec4 color);
const float N_BIT = 3;

void main(){
    vec4 K[9];
    for(int i = 0; i < 9; i++){
        K[i] = texture(screenTexture, TexCoords + kernel[i]);
    }

    switch(effect){
        case 0: // NO_POSTPROD
            fragcolor = K[4];
            break;
        case 1: //AVERAGER
            fragcolor = apply_filter(average_kernel, K);
            break;
        case 2: //GAUSSIAN
            fragcolor = apply_filter(gauss_kernel, K);
            break;
        case 3: //SHARPENER
            fragcolor = apply_filter(sharpen_kernel, K);
            break;
        case 4: //SOBEL
            fragcolor = apply_filter(sobel_kernel, K);
            break;
        case 5: //LAPLACIAN
            fragcolor = apply_filter(laplacian_kernel, K);
            break;
        case 6: //GREYSCALE
            fragcolor = K[4];
            float average = 0.2126 * fragcolor.r + 0.7152 * fragcolor.g + 0.0722 * fragcolor.b;
            fragcolor = vec4(average, average, average, 1.0);
            break;
        case 7: //INVERT
            fragcolor = vec4(1-K[4].rgb, K[4].a);
            break;
        case 8: //BRIGHT
            fragcolor = vec4(K[4].rgb * 4, K[4].a);
            break;
        case 9: //DARK
            fragcolor = vec4(K[4].rgb * 0.3, K[4].a);
            break;
        case 10: //THRESHOLD
            if((K[4].r+K[4].g+K[4].b)/3 > 0.4)
                fragcolor = vec4(1);
            else
                fragcolor = vec4(0,0,0,1);
            break;
        case 11: //RED_CANAL
            fragcolor = vec4(K[4].r,0,0,K[4].a);
            break;
        case 12: //GREEN_CANAL
            fragcolor = vec4(0,K[4].g,0,K[4].a);
            break;
        case 13: //BLUE_CANAL
            fragcolor = vec4(0,0,K[4].b,K[4].a);
            break;
        case 14: //BIT_REDUCE
            fragcolor = get_bit_reduce(N_BIT, K[4]);
            break;
        default:
            fragcolor = K[4];
    }
}

vec4 apply_filter(float filter_kernel[9], vec4 K[9]){
    vec3 col = vec3(0);
    for(int i = 0; i < 9; i++)
        col += K[i].rgb * filter_kernel[i];
    return vec4(col, K[4].a);
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