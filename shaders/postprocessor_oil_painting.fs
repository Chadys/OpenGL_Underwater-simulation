#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D screenTexture;
uniform int effect;

uniform float width;
uniform float height;

vec4 get_oil_painting();

float TILE_V_SIZE = (width/50)/width;
float TILE_H_SIZE = (height/30)/height;

void main(){
    fragcolor = get_oil_painting();
}

//https://github.com/Blackhart/GLSL-Shaders/blob/master/glsl/OilPainting/oilPainting.frag
vec4 get_oil_painting(){
    float   uni_Radius = 7.0;

    vec2    uni_ScreenSize = vec2(width, height);

    float   lRadiusSquare = float((uni_Radius + 1) * (uni_Radius + 1));
    vec3    lColor[4];
    vec3    lColorSquare[4];

    for (int lIndex = 0; lIndex < 4; lIndex++)
    {
        lColor[lIndex] = vec3(0.0f);
        lColorSquare[lIndex] = vec3(0.0f);
    }

    for (int j = int(-uni_Radius); j <= 0; j++)
    {
        for (int i = int(-uni_Radius); i <= 0; i++)
        {
            vec3    lTexColor = texture2D(screenTexture, TexCoords + vec2(i, j) / uni_ScreenSize).rgb;
            lColor[0] += lTexColor;
            lColorSquare[0] += lTexColor * lTexColor;
        }
    }

    for (int j = int(-uni_Radius); j <= 0; j++)
    {
        for (int i = 0; i <= uni_Radius; i++)
        {
            vec3    lTexColor = texture2D(screenTexture, TexCoords + vec2(i, j) / uni_ScreenSize).rgb;
            lColor[1] += lTexColor;
            lColorSquare[1] += lTexColor * lTexColor;
        }
    }

    for (int j = 0; j <= uni_Radius; j++)
    {
        for (int i = 0; i <= uni_Radius; i++)
        {
            vec3    lTexColor = texture2D(screenTexture, TexCoords + vec2(i, j) / uni_ScreenSize).rgb;
            lColor[2] += lTexColor;
            lColorSquare[2] += lTexColor * lTexColor;
        }
    }

    for (int j = 0; j <= uni_Radius; j++)
    {
        for (int i = int(-uni_Radius); i <= 0; i++)
        {
            vec3    lTexColor = texture2D(screenTexture, TexCoords + vec2(i, j) / uni_ScreenSize).rgb;
            lColor[3] += lTexColor;
            lColorSquare[3] += lTexColor * lTexColor;
        }
    }

    float   lMinSigma = 4.71828182846;

    vec4 color;
    for (int i = 0; i < 4; i++)
    {
        lColor[i] /= lRadiusSquare;
        lColorSquare[i] = abs(lColorSquare[i] / lRadiusSquare - lColor[i] * lColor[i]);
        float   lSigma = lColorSquare[i].r + lColorSquare[i].g + lColorSquare[i].b;
        if (lSigma < lMinSigma)
        {
            lMinSigma = lSigma;
            color = vec4(lColor[i], texture2D(screenTexture, TexCoords).a);
        }
    }
    return color;
}