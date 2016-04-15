struct VOut
{
	float4 position : SV_POSITION;
	float3 tangentFragPos : TANGENT0;
	float3 normal : NORMAL0;
	float2 texcoords : TEXCOORD0;
	float3x3 TBNVout : TBN;
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
	float4 fragPos = mul(M, position);
	output.texcoords = texcoords;
	float3x3 normalMatrix = transpose((float3x3)M);
	float3 T = normalize(mul(normalMatrix, tangent));
	float3 B = normalize(mul(normalMatrix, bitangent));
	float3 N = normalize(mul(normalMatrix, normal));

	float3x3 TBN = transpose(float3x3(T, B, N));

	output.tangentFragPos = mul(TBN, fragPos.xyz);

	output.TBNVout = TBN;
	output.normal = normal;

	return output;
}