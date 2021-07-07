struct VS_INPUT
{
	float4 pos : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 camera_direction : TEXCOORD2;
};

cbuffer constant : register(b0)
{
	row_major float4x4 worldMatrix;
	row_major float4x4 viewMatrix;
	row_major float4x4 projectionMatrix;
	float4 light_direction;
	float4 camera_position;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.pos = mul(input.pos, worldMatrix);//OBJECT SPACE TO WORLD SPACE
	output.pos = mul(output.pos, viewMatrix);//WORLD SPACE TO VIEW SPACE
	output.pos = mul(output.pos, projectionMatrix);//VIEW SPACE TO SCREEN SPACE

	output.camera_direction = normalize(output.pos.xyz - camera_position.xyz);

	output.texcoord = input.texcoord;
	output.normal = input.normal;

	return output;
}