Texture2D sTexture;

SamplerState textureSampler;

float4 main(
	float4 position : SV_POSITION, 
	float2 texcoords : TEXCOORD0) : SV_TARGET
{
	return sTexture.Sample(textureSampler, texcoords);
}