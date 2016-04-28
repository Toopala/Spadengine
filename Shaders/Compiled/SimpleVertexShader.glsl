#version 440 core

in vec3 inPosition;
in vec2 inTexCoords;

out vec2 outTexCoords;

layout (std140, binding = 0) uniform vertexUniform
{
	mat4 MVP;
};

void main()
{
	gl_Position = MVP * vec4(inPosition, 1.0);
	outTexCoords = inTexCoords;
}

