Texture2D tex;
Texture2D tex2;

struct VOut
{
	float4 position : SV_POSITION;
	float3 tangentFragPos : TANGENT0;
	float3 tangentLightPos : TANGENT1;
	float3 normal : NORMA0L;
	float2 texcoords : TEXCOORD0;
};

SamplerState textureSampler;

float4 main(VOut vout) : SV_TARGET
{
	float3 normal = normalize(vout.normal);
	normal = tex2.Sample(textureSampler, vout.texcoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	float3 color = tex.Sample(textureSampler, vout.texcoords).rgb;
	float3 ambient = 0.2 * color;

	float3 lightDirection = vout.tangentLightPos - vout.tangentFragPos;

	float distance = length(vout.tangentLightPos - vout.tangentFragPos);
	float attenuation = 1.0 / (1.0 + 0.000009 * distance + 0.0016 * distance * 2);

	float diff = max(0.0, dot(normalize(normal), normalize(lightDirection)));

	float3 diffuse = diff * color;

	return float4(diffuse * attenuation + ambient * attenuation, 1.0);
}