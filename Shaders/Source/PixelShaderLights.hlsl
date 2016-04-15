Texture2D diffuseTex;
Texture2D normalTex;
Texture2D specularTex;
float shininess = 0.5;

#define NUM_POINT_LIGHTS 40

struct VOut
{
	float4 position : SV_POSITION;
	float3 tangentFragPos : TANGENT0;
	float3 normal : NORMAL0;
	float2 texcoords : TEXCOORD0;
	float3x3 TBNVout: TBN;
};

struct DirLight
{
	float3 direction;

	float3 ambient;
	float3 diffuse;
	float3 specular;
};

struct PointLight
{
	float3 position;

	float constant;
	float mylinear;
	float quadratic;

	float3 ambient;
	float3 diffuse;
	float3 specular;
};

float3 CalculateDirectionLight(DirLight light, float3 normal, float3 viewDir, VOut vout);
float3 CalculatePointLight(PointLight light, float3 normal, float3 tangentFragPos, float3 viewDir, VOut vout);

cbuffer UniformData : register(b1)
{
	int numberOfLights;
	DirLight dirLight;
	PointLight pointLights[NUM_POINT_LIGHTS];
	float3 viewPos;
};

SamplerState textureSampler;

float4 main(VOut vout) : SV_TARGET
{
	PointLight pointLights2[NUM_POINT_LIGHTS];
	pointLights2[0].position = float3(1.0, 2.0, 0.0);
	pointLights2[0].constant = float(1.0);
	pointLights2[0].mylinear = float(0.09);
	pointLights2[0].quadratic = float(0.032);
	pointLights2[0].ambient = float3(0.05, 0.05, 0.05);
	pointLights2[0].diffuse = float3(0.8, 0.8, 0.8);
	pointLights2[0].specular = float3(1.0, 1.0, 1.0);
	
	DirLight dirLight2;

	dirLight2.direction = float3(0.0f, -1.0f, 0.0f);
	dirLight2.ambient = float3(0.05, 0.05, 0.05);
	dirLight2.diffuse = float3(0.8, 0.8, 0.8);
	dirLight2.specular = float3(0.5, 0.5, 0.5);

	float3 normal = normalize(vout.normal);
	normal = normalTex.Sample(textureSampler, vout.texcoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	float3 TangentViewPos = mul(vout.TBNVout, viewPos);
		float3 viewDir = normalize(TangentViewPos - vout.tangentFragPos);

		float3 result = CalculateDirectionLight(dirLight2, normal, viewDir, vout);
	
		for (int i = 0; i < numberOfLights; i++)
			result += CalculatePointLight(pointLights2[i], normal, vout.tangentFragPos, viewDir, vout);

	return float4(result, 1.0);
}

float3 CalculateDirectionLight(DirLight light, float3 normal, float3 viewDir, VOut vout)
{
		float3 tangentLightDir = mul(-light.direction, vout.TBNVout);
		float3 lightDir = normalize(tangentLightDir);
		// Diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
		float3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// Combine results
	float3 ambient = light.ambient * diffuseTex.Sample(textureSampler, vout.texcoords).rgb;
		float3 diffuse = light.diffuse * diff * diffuseTex.Sample(textureSampler, vout.texcoords).rgb;
		float3 specular = light.specular * spec * specularTex.Sample(textureSampler, vout.texcoords).rgb;
		return (ambient + diffuse + specular);
}

float3 CalculatePointLight(PointLight light, float3 normal, float3 tangentFragPos, float3 viewDir, VOut vout)
{
	float3 lightPosTBN = mul(vout.TBNVout, light.position);
		float3 lightDir = normalize(lightPosTBN - vout.tangentFragPos);
		// Diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	float3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// Attenuation
	float distance = length(lightPosTBN - vout.tangentFragPos);
	float attenuation = 1.0f / (light.constant + light.mylinear * distance + light.quadratic * (distance * distance));
	// Combine results
	float3 ambient = light.ambient * diffuseTex.Sample(textureSampler, vout.texcoords).rgb;
		float3 diffuse = light.diffuse * diff * diffuseTex.Sample(textureSampler, vout.texcoords).rgb;
		float3 specular = light.specular * spec * specularTex.Sample(textureSampler, vout.texcoords).rgb;
		ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}