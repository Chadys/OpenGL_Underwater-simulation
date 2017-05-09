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