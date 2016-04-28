#version 440 core
in vec2 outTexCoords;
out vec4 finalColor;

layout(binding = 0) uniform sampler2D texture;

layout (std140, binding = 1) uniform pixelUniform
{
	vec4 color;
};

void main()
{
	finalColor = texture2D(texture, outTexCoords) * color;
}