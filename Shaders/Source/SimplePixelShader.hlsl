Texture2D tex;

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
    float2 texcoords : TEXCOORD0;
};

SamplerState textureSampler;

float4 main(VOut vout) : SV_TARGET
{
    return vout.color * tex.Sample(textureSampler, vout.texcoords);
}