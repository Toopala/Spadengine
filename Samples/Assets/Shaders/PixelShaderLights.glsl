#version 440 core

in vec2 texcoords;
in vec3 normals;
in vec3 fragPosition;
in mat3 TBNVout;
in float shininessVout;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D diffuseTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform sampler2D specularTex;

#define NUM_POINT_LIGHTS 40
#define NUM_DIR_LIGHTS 10
#define NUM_SPOT_LIGHTS 0

//float shininess = 100.0;

struct DirLight
{
	vec4 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct PointLight
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	
	float constant;
	float mylinear;
	float quadratic;
	float pad2;
	
};

layout(binding = 1, std140) uniform pixelUniform
{
	DirLight dirLight[NUM_DIR_LIGHTS];
	PointLight pointLights[NUM_POINT_LIGHTS];
	vec4 viewPos;
	float numofpl;
	float numofdl;
	float numofsl;
	float pad;
};

vec3 CalculateDirectionLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir);

void main()		
{
	int dl = int(numofdl);
	int pl = int(numofpl);
	
	vec3 normal = normalize(normals);
	normal = texture(normalTex, texcoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	
	vec3 viewDir = TBNVout * normalize(viewPos.xyz - fragPosition);
    vec3 result = vec3(0.0);
	
	for(int i = 0; i < dl; i++)
		result += CalculateDirectionLight(dirLight[i], normal, viewDir);
	
	for(int i = 0; i < pl; i++)
		result += CalculatePointLight(pointLights[i], normal, viewDir);
    
	outColor = vec4(result, 1.0);
}

vec3 CalculateDirectionLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = TBNVout * normalize(-light.direction.xyz);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessVout);
	// Combine results
	vec3 ambient = light.ambient.xyz * texture(diffuseTex, texcoords).rgb;
	vec3 diffuse = light.diffuse.xyz * diff * texture(diffuseTex, texcoords).rgb;
	vec3 specular = light.specular.xyz * spec * texture(specularTex, texcoords).rgb;
	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = TBNVout * normalize(light.position.xyz - fragPosition);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessVout);
	// Attenuation
	float distance = length(light.position.xyz - fragPosition);
	float attenuation = 1.0f / (light.constant + light.mylinear * distance + light.quadratic * (distance * distance));
	// Combine results
	vec3 ambient = light.ambient.xyz * texture(diffuseTex, texcoords).rgb;
	vec3 diffuse = light.diffuse.xyz * diff * texture(diffuseTex, texcoords).rgb;
	vec3 specular = light.specular.xyz * spec * texture(specularTex, texcoords).rgb;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
