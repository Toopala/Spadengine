#version 440 core

layout(location = 0) in vec3 inPosition;
layout(location = 4) in vec2 inTexcoords;

out vec2 texcoords;

void main()
{
	gl_Position = PV * M * vec4(inPosition, 1.0);
	texcoords = inTexcoords;
}
