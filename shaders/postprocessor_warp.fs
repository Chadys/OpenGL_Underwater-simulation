#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D screenTexture;
uniform int effect;

uniform float width;
uniform float height;

vec4 get_warp();


void main(){
    fragcolor = get_warp();
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