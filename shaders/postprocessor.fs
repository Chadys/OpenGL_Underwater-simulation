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
uniform float width;
uniform float height;

vec4 apply_filter(float filter_kernel[9], vec4 K[9]);
vec4 get_bit_reduce(float n_bits, vec4 color);
vec4 get_mosaic();
vec4 get_halftone(vec4 color);
vec4 get_swirl();
vec4 get_warp();
vec4 get_fisheye();
vec4 get_kalei();
vec4 get_kalei2();
vec4 get_oil_painting();
vec4 get_sketch(vec4 K[9]);

float rand(vec2 co){
    return fract(sin(dot(co ,vec2(12.9898,78.233))) * 43758.5453);
}

const float PI = 3.1415926535;
const float N_BIT = 3;
float TILE_V_SIZE = (width/50)/width;
float TILE_H_SIZE = (height/30)/height;

void main(){
    vec4 K[9];
    for(int i = 0; i < 9; i++){
        K[i] = texture(screenTexture, TexCoords + kernel[i]);
    }
    vec4 mosaic_col = get_mosaic();
    vec4 swirl_col = get_swirl();
    vec4 warp_col = get_warp();
    vec4 fisheye_col = get_fisheye();
    vec4 kalei_col = get_kalei();
    vec4 kalei_col2 = get_kalei2();
    vec4 oil_color = get_oil_painting();
    vec4 sketch_color = get_sketch(K);

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
        case 15: //MOSAIC
            fragcolor = mosaic_col;
            break;
        case 16: //OIL_PAINTING
            fragcolor = oil_color;
            break;
        case 17: //SKETCH
            fragcolor = sketch_color;
            break;
        case 18: //HALFTONING
            fragcolor = get_halftone(K[4]);
            break;
        case 19: //SWIRL
            fragcolor = swirl_col;
            break;
        case 20: //FISHEYE
            fragcolor = fisheye_col;
            break;
        case 21: //TIMEWARP
            fragcolor = warp_col;
            break;
        case 22: //KALEIDOSCOPE
            fragcolor = kalei_col;
            break;
        case 23: //KALEIDOSCOPE2
            fragcolor = kalei_col2;
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

vec4 get_mosaic(){
    vec2 coord = vec2(0);
    while(TexCoords.x-coord.x > TILE_V_SIZE/2){
        coord.x += TILE_V_SIZE;
    }
    while(TexCoords.y-coord.y > TILE_H_SIZE/2){
        coord.y += TILE_H_SIZE;
    }
    return texture(screenTexture, coord);
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

//http://www.geeks3d.com/20140213/glsl-shader-library-fish-eye-and-dome-and-barrel-distortion-post-processing-filters/5/
vec4 get_warp(){
  //normalized coords with some cheat
  vec2 p = gl_FragCoord.xy / width;

  //screen proportion
  float prop = width / height;
  //center coords
  vec2 m = vec2(0.5, 0.5 / prop);
  //vector from center to current fragment
  vec2 d = p - m;
  // distance of pixel from center
  float r = sqrt(dot(d, d));
  //amount of effect
  float power = ( 2.0 * 3.141592 / (2.0 * sqrt(dot(m, m))) ) * (1.0 / width - 0.5);
  //radius of 1:1 effect
  float bind;
  if (power > 0.0) bind = sqrt(dot(m, m));//stick to corners
  else {if (prop < 1.0) bind = m.x; else bind = m.y;}//stick to borders

  //Weird formulas
  vec2 uv;
  if (power > 0.0)//fisheye
    uv = m + normalize(d) * tan(r * power) * bind / tan( bind * power);
  else if (power < 0.0)//antifisheye
   uv = m + normalize(d) * atan(r * -power * 10.0) * bind / atan(-power * bind * 10.0);
  else
    uv = p;//no effect for power = 1.0

  //Second part of cheat
  //for round effect, not elliptical
  return texture(screenTexture, vec2(uv.x, -uv.y * prop));
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

//https://github.com/Blackhart/GLSL-Shaders/blob/master/glsl/OilPainting/oilPainting.frag
vec4 get_oil_painting(){
    float   uni_Radius = 10.0;

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