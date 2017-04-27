#version 330 core
in vec2 TexCoord;

out vec4 fragcolor;

uniform sampler2D sprite;

uniform vec3 spriteColor;
uniform float spriteAlpha;

uniform bool border;

uniform int effect;
uniform bool isCircle;
uniform vec3 color1;
uniform vec3 color2;

uniform float fade;


vec4 create_border(vec4 color);
vec3 create_gradient();
vec3 get_life();
bool inCircle();


void main()
{
	vec4 color;
    color = texture(sprite, TexCoord);
	if(color.a<0.1)
		discard;
    fragcolor = color*fade;
}

bool inCircle(){
	if (distance(TexCoord, vec2(0.5,0.5)) > 0.5)
		return false;
	return true;
}

vec3 create_gradient(){
	switch(effect){

	// NONE
	case 0 :
		return color1;

	// RADIAL, color1 is centered
	case 1 :
		return mix(color1, color2, min(distance(TexCoord, vec2(0.5,0.5)), 1));

	// VERTICAL, color1 is up
	case 2 :
		return mix(color1, color2, distance(TexCoord, vec2(TexCoord.x,0.0)));

	// HORIZONTAL, color1 is on the left
	case 3 :
		return mix(color1, color2, distance(TexCoord, vec2(0.0,TexCoord.y)));
	}
}

vec4 create_border(vec4 color){
	if (TexCoord.x < 0.01 || TexCoord.x > 0.99 || TexCoord.y < 0.01 || TexCoord.y > 0.99)
		return vec4(vec3(0), 1);
	return color;
}