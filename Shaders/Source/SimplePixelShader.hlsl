Texture2D tex;

struct VOut
{
	float4 position : SV_POSITION;
    float2 texcoords : TEXCOORD0;
};

cbuffer UniformData : register(b1)
{
    float4 color;
}

SamplerState textureSampler;

float4 main(VOut vout) : SV_TARGET
{
    return color * tex.Sample(textureSampler, vout.texcoords);
}