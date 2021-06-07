struct VS_INPUT
{
	float4 pos : POSITION;
	float4 pos1 : POSITION1;
	float3 color : COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

cbuffer constant : register(b0)
{
	unsigned int time;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = lerp(input.pos, input.pos1, (sin(constant.time/1000.0)+1.0)/2.0);
	output.color = input.color;
	return output;
}