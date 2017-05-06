#version 330 core
in vec2 TexCoord;

out vec4 fragcolor;

uniform sampler2D bubble;

uniform float fade;

void main()
{
	vec4 color;
    color = texture(bubble, TexCoord);
	if(color.a<0.001)
		discard;
    fragcolor = color*fade;
}