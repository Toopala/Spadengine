#version 440 core

in vec2 texcoords;

layout(location = 0) out vec4 outColor;
layout(binding = 3) uniform samplerCube cubeTex;

void main()		
{  
	outColor = texture(cubeTex, vec3(texcoords, 1.0)).rgb;
}
