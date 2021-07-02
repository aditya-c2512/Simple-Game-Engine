Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

cbuffer constant : register(b0)
{
	row_major float4x4 worldMatrix;
	row_major float4x4 viewMatrix;
	row_major float4x4 projectionMatrix;
	unsigned int time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	return Texture.Sample(TextureSampler, input.texcoord*0.5);
}