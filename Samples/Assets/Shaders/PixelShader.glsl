#version 440

in vec2 texcoords;
in vec3 normals;
in vec3 tangentLightPos;
in vec3 tangentFragPos;

out vec4 outColor;

layout(binding = 0) uniform sampler2D tex;
layout(binding = 1) uniform sampler2D tex2;

void main()
{
	vec3 normal = normalize(normals);
	normal = texture(tex2, texcoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	
	vec3 color = texture(tex, texcoords).rgb;
	vec3 ambient = 0.2 * color;
	
	vec3 lightDirection = tangentLightPos - tangentFragPos;
	
	float distance = length(tangentLightPos - tangentFragPos);
	float attenuation = 1.0 / (1.0 + 0.0000009 * distance + 0.0016 * (distance * distance));
	
	float diff = max(0.0, dot(normalize(normal), normalize(lightDirection)));
	vec3 diffuse = diff * color;
	outColor = vec4(diffuse * attenuation + ambient * attenuation, 1.0);
}