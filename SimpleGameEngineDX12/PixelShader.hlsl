struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

cbuffer constant : register(b0)
{
	unsigned int time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	return float4(lerp(input.color, input.color1,(sin(time / 500.0) + 1.0) / 2.0), 1.0f);
}