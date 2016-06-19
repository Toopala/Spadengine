#version 440 core
//#extension GL_NV_shadow_samplers_cube : enable

in vec2 texcoords;
in vec3 normals;
in vec3 fragPosition;
in mat3 TBNVout;
in float shininessVout;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D diffuseTex;
layout(binding = 1) uniform sampler2D normalTex;
layout(binding = 2) uniform sampler2D specularTex;
layout(binding = 3) uniform samplerCube cubeTex;

#define NUM_POINT_LIGHTS 40
#define NUM_DIR_LIGHTS 10
#define NUM_SPOT_LIGHTS 0

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
	vec4 viewPos;
	int hasDiffuseTex;
	int hasNormalTex;
	int hasSpecularTex;
	int hasCubeTex;
	float glossyness;
	float pad[3];
};

layout(binding = 2, std140) uniform lightUniform
{
	DirLight dirLight[NUM_DIR_LIGHTS];
	PointLight pointLights[NUM_POINT_LIGHTS];
	float numofpl;
	float numofdl;
	float numofsl;
	float pad2;
};

vec3 CalculateDirectionLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir);

void main()		
{
	int dl = int(numofdl);
	int pl = int(numofpl);
	
	vec3 normal = vec3(0.0);
	vec3 viewDir= vec3(0.0);
	//normal texture
	if(hasNormalTex == 1)
	{
		normal = normalize(normals);
		normal = texture(normalTex, texcoords).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		
		viewDir = TBNVout * normalize(viewPos.xyz - fragPosition);
	}
	else
	{
		normal = normalize(normals);
		viewDir = normalize(viewPos.xyz - fragPosition);
	}	
	
    vec3 result = vec3(0.0);
	
	//Lights
	for(int i = 0; i < dl; i++)
		result += CalculateDirectionLight(dirLight[i], normal, viewDir);
	
	for(int i = 0; i < pl; i++)
		result += CalculatePointLight(pointLights[i], normal, viewDir);
	
	//Cubemap TODO: fix
	vec3 I = vec3(0.0);
	vec3 R = vec3(0.0);
	vec4 cubeColor = vec4(0.0);
	float glossyFactor = 0.0;
	vec4 glossyColor = vec4(0.0);
	vec4 resultCube = vec4(0.0);
	if(hasCubeTex == 1)
	{
		I = viewDir;
		R = reflect(I, normal);
		cubeColor = texture(cubeTex, R);
		if(hasSpecularTex == 1)
		{
			glossyColor = texture(specularTex, texcoords);
			glossyFactor = glossyness * glossyColor.x;
		}
		else
		{
			glossyFactor = glossyness;
		}		
		
		resultCube = (1.0-glossyFactor)* vec4(result, 1.0) + (glossyFactor)*cubeColor;
		outColor = resultCube;
	}
	else
	{
		outColor = vec4(result, 1.0);
	}
}

vec3 CalculateDirectionLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir= vec3(0.0);
	if(hasNormalTex == 1)
	{
		lightDir = TBNVout * normalize(-light.direction.xyz);
	}
	else
	{
		lightDir = normalize(-light.direction.xyz);
	}
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	//Blinn phong specular shading
	vec3 halfDir = normalize(lightDir + viewDir);
	float specAngle = max(dot(halfDir, normal), 0.0);
	float spec = pow(specAngle, shininessVout);
	
	// Combine results
	vec3 ambient = light.ambient.xyz * texture(diffuseTex, texcoords).rgb;
	vec3 diffuse = light.diffuse.xyz * diff * texture(diffuseTex, texcoords).rgb;
	vec3 specular = vec3(0.0);
	if(hasSpecularTex == 1)
	{	
		specular = light.specular.xyz * spec * texture(specularTex, texcoords).rgb;
	}
	else
	{
		specular = light.specular.xyz * spec;
	}
	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir= vec3(0.0);
	if(hasNormalTex == 1)
	{
		lightDir = TBNVout * normalize(light.position.xyz - fragPosition);
	}
	else
	{
		lightDir = normalize(light.position.xyz - fragPosition);
	}
	
	//Blinn phong specular shading
	vec3 halfDir = normalize(lightDir + viewDir);
	float specAngle = max(dot(halfDir, normal), 0.0);
	float spec = pow(specAngle, shininessVout);
	
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);	
	
	// Attenuation
	float distance = length(light.position.xyz - fragPosition);
	float attenuation = 1.0f / (light.constant + light.mylinear * distance + light.quadratic * (distance * distance));
	// Combine results
	vec3 ambient = light.ambient.xyz * texture(diffuseTex, texcoords).rgb;
	vec3 diffuse = light.diffuse.xyz * diff * texture(diffuseTex, texcoords).rgb;
	vec3 specular = vec3(0.0);
	if(hasSpecularTex == 1)
	{
		specular = light.specular.xyz * spec * texture(specularTex, texcoords).rgb;
	}
	else
	{
		specular = light.specular.xyz * spec;
	}
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
