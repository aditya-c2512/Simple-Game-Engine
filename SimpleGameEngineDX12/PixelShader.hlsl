#define PI 3.14159

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

float G_GGX(float a, float3 n, float3 l, float3 v)
{
	float k = pow(a + 1, 2) / 8.0f;
	float Gl = 1.0f / (dot(n, l) * (1 - k) + k);
	float Gv = 1.0f / (dot(n, v) * (1 - k) + k);
	return Gl * Gv;
}
float F_GGX(float f0, float voh)
{
	float f = (f0 + (1.0f - f0) * pow(2.0f,(-5.55473*voh-6.98316)*voh));
	return f;
}
float D_GGX(float a, float noh)
{
	float d = a / (noh * noh * (a * a - 1.0f) + 1.0f);
	return d * d;
}

float3 specPhong(float3 n, float3 l, float3 v, float glossy)
{
	//Phong Shading
	float ks = 1.0f;
	float3 is = float3(1, 1, 1);
	float3 specular = ks * pow(max(dot(reflect(l, n), v),0),glossy) * is; 
	return specular;
}

float3 specBlinnPhong(float3 n, float3 l, float3 v, float glossy)
{
	//Blinn-Phong Shading
	float ks = 1.0f;
	float3 is = float3(1, 1, 1);
	float3 halfway = normalize(v + l);
	float3 specular = ks * (glossy + 8) / 8 * pow(max(dot(n, halfway), 0), glossy) * is;
	return specular;
}

float3 specGGX(float3 n, float3 l, float3 v, float roughness)
{
	//Cook-Torrance GGX Shading
	float ks = 1.0f;
	float3 halfway = normalize(v + l);
	float3 is = float3(1, 1, 1);
	float3 specular = ks * is * max(dot(l, n), 0) * D_GGX(roughness * roughness, dot(n, halfway)) * F_GGX(0.9f, dot(l, halfway)) * G_GGX(roughness, n, l, v);
	return specular;
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float ka = 0.1f;
	float3 ia = float3(1, 1, 1);
	float3 ambient = ka * ia;

	float kd = 1.0f;
	//float3 id = float3(1, 1, 1);
	float3 id = (Texture.Sample(TextureSampler, input.texcoord)).xyz;
	float3 halfway = normalize(-input.camera_direction + light_direction.xyz);
	float3 diffuse = kd * max(dot(light_direction.xyz, input.normal), 0) * id;
	// * (1.0f - F_GGX(0.1f, dot(light_direction.xyz, halfway)))

	float3 specular = specGGX(input.normal, light_direction.xyz, -input.camera_direction, 0.05f);

	float3 final_color = ambient + diffuse + specular;

	return float4(final_color, 1.0);
}