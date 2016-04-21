#version 440

in vec2 texcoords;
in vec3 normals;
in vec3 tangentFragPos;
in mat3 TBNVout;

out vec4 outColor;

layout(binding = 0) uniform sampler2D diffuseTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform sampler2D specularTex;

#define NUM_POINT_LIGHTS 40

float shininess = 0.5;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float constant;
	float mylinear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (std140, binding = 1) uniform pixelUniform
{
	int numberOfLights;
	DirLight dirLight;
	PointLight pointLights[NUM_POINT_LIGHTS];
	vec3 viewPos;
};

vec3 CalculateDirectionLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir);

void main()
{
	PointLight pointLights2[NUM_POINT_LIGHTS];
	pointLights2[0].position = vec3(1.0, 2.0, 0.0);
	pointLights2[0].constant = float(1.0);
	pointLights2[0].mylinear = float(0.09);
	pointLights2[0].quadratic = float(0.032);
	pointLights2[0].ambient = vec3(0.05, 0.05, 0.05);
	pointLights2[0].diffuse = vec3(0.8, 0.8, 0.8);
	pointLights2[0].specular = vec3(1.0, 1.0, 1.0);
	
	DirLight dirLight2;

	dirLight2.direction = vec3(0.0f, -1.0f, 0.0f);
	dirLight2.ambient = vec3(0.05, 0.05, 0.05);
	dirLight2.diffuse = vec3(0.8, 0.8, 0.8);
	dirLight2.specular = vec3(0.5, 0.5, 0.5);

	vec3 normal = normalize(normals);
	normal = texture(normalTex, texcoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 TangentViewPos = TBNVout * viewPos;
		vec3 viewDir = normalize(TangentViewPos - tangentFragPos);

		vec3 result = CalculateDirectionLight(dirLight2, normal, viewDir);
	
		for (int i = 0; i < numberOfLights; i++)
			//result += CalculatePointLight(pointLights2[i], normal, viewDir);

	outColor = vec4(result, 1.0);
}

vec3 CalculateDirectionLight(DirLight light, vec3 normal, vec3 viewDir)
{
		vec3 tangentLightDir = -light.direction * TBNVout;
		vec3 lightDir = normalize(tangentLightDir - tangentFragPos);
		// Diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// Combine results
	vec3 ambient = light.ambient * texture(diffuseTex, texcoords).rgb;
		vec3 diffuse = light.diffuse * diff * texture(diffuseTex, texcoords).rgb;
		vec3 specular = light.specular * spec * texture(specularTex, texcoords).rgb;
		return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightPosTBN = TBNVout * light.position;
		vec3 lightDir = normalize(lightPosTBN - tangentFragPos);
		// Diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// Attenuation
	float distance = length(lightPosTBN - tangentFragPos);
	float attenuation = 1.0f / (light.constant + light.mylinear * distance + light.quadratic * (distance * distance));
	// Combine results
	vec3 ambient = light.ambient * texture(diffuseTex, texcoords).rgb;
		vec3 diffuse = light.diffuse * diff * texture(diffuseTex, texcoords).rgb;
		vec3 specular = light.specular * spec * texture(specularTex, texcoords).rgb;
		ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
