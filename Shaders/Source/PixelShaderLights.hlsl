 Texture2D diffuseTex;
Texture2D normalTex;
Texture2D specularTex;
float shininess = 0.9;

#define NUM_POINT_LIGHTS 40

struct VOut
{
	float4 position : SV_POSITION;
	float3 fragPos : TANGENT0;
	float3 normal : NORMAL0;
	float2 texcoords : TEXCOORD0;
	float3x3 TBNVout: TBN;
};

struct DirLight
{
	float4 direction;

	float4 ambient;
	float4 diffuse;
	float4 specular;
};

struct PointLight
{
	float4 position;

	float constant;
	float mylinear;
	float quadratic;

	float4 ambient;
	float4 diffuse;
	float4 specular;
	float pad;
};

float3 CalculateDirectionLight(DirLight light, float3 normal, float3 viewDir, VOut vout);
float3 CalculatePointLight(PointLight light, float3 normal, float3 fragPos, float3 viewDir, VOut vout);

cbuffer UniformData : register(b1)
{
	DirLight dirLight;
	PointLight pointLights[NUM_POINT_LIGHTS];
	float4 viewPos;
	int numofpl;
	int pad[3];
};

SamplerState textureSampler;

float4 main(VOut vout) : SV_TARGET
{
	float3 normal = normalize(vout.normal);
	normal = normalTex.Sample(textureSampler, vout.texcoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	
	float3 viewDir = mul(vout.TBNVout , normalize(viewPos.xyz - vout.fragPos));

	float3 result = CalculateDirectionLight(dirLight, normal, viewDir, vout);
	
	for (int i = 0; i < numofpl; i++)
		result += CalculatePointLight(pointLights[i], normal, vout.fragPos, viewDir, vout);

	return float4(result, 1.0);
}

float3 CalculateDirectionLight(DirLight light, float3 normal, float3 viewDir, VOut vout)
{
	float3 lightDir = mul(vout.TBNVout , normalize(-light.direction.xyz));
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	float3 reflectDir = mul(vout.TBNVout , reflect(-lightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// Combine results
	float3 ambient = light.ambient.xyz * diffuseTex.Sample(textureSampler, vout.texcoords).rgb;
	float3 diffuse = light.diffuse.xyz * diff * diffuseTex.Sample(textureSampler, vout.texcoords).rgb;
	float3 specular = light.specular.xyz * spec * specularTex.Sample(textureSampler, vout.texcoords).rgb;
	return (ambient + diffuse + specular);
}

float3 CalculatePointLight(PointLight light, float3 normal, float3 fragPos, float3 viewDir, VOut vout)
{
	float3 lightDir = mul(vout.TBNVout , normalize(light.position.xyz - fragPos));
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	float3 reflectDir = mul(vout.TBNVout , reflect(-lightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// Attenuation
	float distance = length(light.position.xyz - fragPos);
	float attenuation = 1.0f / (light.constant + light.mylinear * distance + light.quadratic * (distance * distance));
	// Combine results
	float3 ambient = light.ambient.xyz * diffuseTex.Sample(textureSampler, vout.texcoords).rgb;
	float3 diffuse = light.diffuse.xyz * diff * diffuseTex.Sample(textureSampler, vout.texcoords).rgb;
	float3 specular = light.specular.xyz * spec * specularTex.Sample(textureSampler, vout.texcoords).rgb;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}