struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer UniformData : register(b0)
{
	float4x4 MVP;
	float4 color;
}

VOut main(
	float4 position : POSITION0)
{
	VOut output;

	output.position = mul(MVP, position);
	output.color = color;

	return output;
}