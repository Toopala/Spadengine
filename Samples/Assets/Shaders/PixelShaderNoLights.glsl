#version 440 core

in vec2 texcoords;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D diffuseTex;

void main()		
{
	outColor = texture(diffuseTex, texcoords);
}