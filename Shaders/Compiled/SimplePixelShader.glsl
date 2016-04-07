#version 440

in vec4 outColor;
in vec2 outTexCoords;

layout(binding = 0) uniform sampler2D texture;

void main()
{
	gl_FragColor = texture2D(texture, outTexCoords) * outColor;
}