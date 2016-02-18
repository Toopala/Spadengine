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

float4 main(VOut vout) : TARGET
{

	return float4(1.0, 1.0, 1.0, 1.0);
}