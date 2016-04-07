#version 440

in vec3 inPosition;
in vec2 inTexCoords;

out vec4 outColor;
out vec2 outTexCoords;

layout (std140, binding = 0) uniform MVPUniform
{
	mat4 MVP;
	vec4 color;
};

void main()
{
	gl_Position = MVP * vec4(inPosition, 1.0);
	outColor = color;
	outTexCoords = inTexCoords;
}
