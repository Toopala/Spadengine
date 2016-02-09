struct VOut
{
	float4 position : SV_POSITION;
};

cbuffer UniformData : register(b0)
{
	float4x4 PV;
	float4x4 M;
}

VOut main(
	float4 position : POSITION, 
	float3 normal : NORMAL, 
	float3 tangent : TANGENT, 
	float3 bitangent : TANGENT,
	float2 texcoords : TEXCOORD)
{
	VOut output;

	output.position = mul(PV, mul(M, position));

	return output;
}