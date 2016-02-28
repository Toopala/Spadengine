struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

//cbuffer UniformData : register(b0)
//{
//	float4x4 PV;
//	float4x4 M;
//}

VOut main(
	float4 position : POSITION0,
	float4 color : COLOR0)
{
	VOut output;

	//output.position = mul(PV, mul(M, position));
	output.position = position;
	output.color = color;

	return output;
}