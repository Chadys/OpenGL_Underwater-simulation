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
vec4 get_sketch(vec4 K[9]);

void main(){
    vec4 K[9];
    for(int i = 0; i < 9; i++){
        K[i] = texture(screenTexture, TexCoords + kernel[i]);
    }

    switch(effect){
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
        case 17: //SKETCH
            fragcolor = get_sketch(K);
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

//https://github.com/Blackhart/GLSL-Shaders/blob/master/glsl/Sketch/sketch.frag
vec4 get_sketch(vec4 K[9]){
    float uni_Opacity = 0.5;
    float uni_Intensity = 0.8;
    vec3 lDefaultIntensity = vec3(0.2125f, 0.7154f, 0.0721f);
    float edge[9];

    for (int i = 0 ; i<9 ; i++){
        if (i != 4)
            edge[i] = dot(K[i].rgb, lDefaultIntensity);
    }

    float lSumColor0 = -edge[0] - 2.0f * edge[1] - edge[2] + edge[6] + 2.0f * edge[7] + edge[8];
    float lSumColor1 = -edge[6] - 2.0f * edge[3] - edge[0] + edge[8] + 2.0f * edge[5] + edge[2];
    highp vec3 lGray = vec3(1.0f - length(vec2(lSumColor0, lSumColor1)));
    return vec4(mix(K[4].rgb, mix(K[4].rgb, lGray, uni_Intensity), uni_Opacity), K[4].a);
}