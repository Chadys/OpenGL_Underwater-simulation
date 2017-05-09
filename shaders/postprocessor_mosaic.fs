#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D screenTexture;
uniform int effect;

uniform float width;
uniform float height;

vec4 get_mosaic();

float TILE_V_SIZE = (width/50)/width;
float TILE_H_SIZE = (height/30)/height;

void main(){
    fragcolor = get_mosaic();
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