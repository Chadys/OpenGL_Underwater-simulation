#version 330 core
in vec2 TexCoords;
out vec4 fragcolor;

uniform sampler2D screenTexture;

void main(){
    fragcolor = texture(screenTexture, TexCoords);
//    fragcolor = vec4(1,0,0,1);
}