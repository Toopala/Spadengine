struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
    float2 texcoords : TEXCOORD0;
};

cbuffer UniformData : register(b0)
{
	float4x4 MVP;
	float4 color;
}

VOut main(
	float4 position : POSITION0,
    float2 texcoords : TEXCOORD0)
{
	VOut output;

	output.position = mul(MVP, position);
	output.color = color;
    output.texcoords = texcoords;

	return output;
}