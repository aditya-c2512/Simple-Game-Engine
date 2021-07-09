Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
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
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float ka = 0.1;
	float3 ia = float3(1, 1, 1);
	float3 ambient = ka * ia;

	float kd = 1.0f;
	//float3 id = float3(1, 0, 0);
	float3 id = (Texture.Sample(TextureSampler, input.texcoord * 0.5)).xyz;
	float3 diffuse = kd * max(dot(light_direction.xyz, input.normal), 0) * id;

	float ks = 2.0f;
	float3 is = float3(1, 1, 1);
	float glossy = 100.0f;
	float3 halfway = normalize(-input.camera_direction + light_direction.xyz);
	//float3 specular = ks * pow(max(dot(reflect(light_direction.xyz, input.normal), input.camera_direction),0),glossy) * is; //Phong Shading
	float3 specular = ks * pow(max(dot(input.normal, halfway),0), glossy) * is; //Blinn-Phong Shading

	float3 final_color = ambient + diffuse + specular;

	return float4(final_color, 1.0);
	//return Texture.Sample(TextureSampler, input.texcoord*0.5);
}