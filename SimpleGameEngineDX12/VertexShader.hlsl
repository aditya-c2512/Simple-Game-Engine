struct VS_INPUT
{
	float4 pos : POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 color1 : COLOR1;
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

	output.color = input.color;
	output.color1 = input.color1;

	return output;
}