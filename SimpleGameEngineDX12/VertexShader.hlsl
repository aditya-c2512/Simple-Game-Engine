struct VS_INPUT
{
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD0;
};

struct VS_OUTPUT
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

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.pos = mul(input.pos, worldMatrix);//OBJECT SPACE TO WORLD SPACE
	output.pos = mul(output.pos, viewMatrix);//WORLD SPACE TO VIEW SPACE
	output.pos = mul(output.pos, projectionMatrix);//VIEW SPACE TO SCREEN SPACE

	output.texcoord = input.texcoord;

	return output;
}