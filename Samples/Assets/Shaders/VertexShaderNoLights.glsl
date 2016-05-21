#version 440 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBitangent;
layout(location = 4) in vec2 inTexcoords;

out vec2 texcoords;

layout (std140, binding = 0) uniform MVPUniform
{
	mat4 PV;
	mat4 M;
	float shininess;
};

void main()
{
	gl_Position = PV * M * vec4(inPosition, 1.0);
	texcoords = inTexcoords;
}
