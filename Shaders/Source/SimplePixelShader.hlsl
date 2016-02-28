struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

SamplerState textureSampler;

float4 main(VOut vout) : SV_TARGET
{
	return vout.color;
}