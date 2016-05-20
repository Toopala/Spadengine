#version 440 core

in vec3 texcoords;

layout(location = 0) out vec4 outColor;

layout(binding = 3) uniform samplerCube cubeTex;

void main()		
{
	outColor = texture(cubeTex, texcoords);
}
