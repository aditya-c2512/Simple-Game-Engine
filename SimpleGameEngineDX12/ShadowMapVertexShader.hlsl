struct VS_INPUT
{
	float4 pos : POSITION0;
};
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
};
cbuffer constant : register(b0)
{
	row_major float4x4 worldMatrix;
	row_major float4x4 viewMatrix;
	row_major float4x4 projectionMatrix;
	float4 light_direction;
	float4 camera_position;
	row_major float4x4 lightSpace;
	float time;
};
VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.pos = mul(input.pos, worldMatrix);//OBJECT SPACE TO WORLD SPACE
	output.pos = mul(output.pos, lightSpace);

	return output;
}