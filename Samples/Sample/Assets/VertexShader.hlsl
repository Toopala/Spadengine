struct VOut
{
	float4 position : SV_POSITION;
	float2 texcoords : TEXCOORD0;
};

cbuffer UniformData : register(b0)
{
	float4x4 PV;
	float4x4 M;
}

VOut main(
	float4 position : POSITION0, 
	float3 normal : NORMAL0, 
	float3 tangent : TANGENT0, 
	float3 bitangent : TANGENT1,
	float2 texcoords : TEXCOORD0)
{
	VOut output;

	output.position = mul(PV, mul(M, position));
	output.texcoords = texcoords;

	return output;
}