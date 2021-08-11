Texture2D tex : register(t0);
sampler TextureSampler : register(s0);

Texture2D shadowMap : register(t1);
sampler ShadowMapSampler : register(s1);

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 camera_direction : TEXCOORD2;
	float4 pixelPosLightSpace : NORMAL1;
};

cbuffer constant : register(b0)
{
	row_major float4x4 worldMatrix;
	row_major float4x4 viewMatrix;
	row_major float4x4 projectionMatrix;
	row_major float4x4 lightSpace;
	float4 light_direction;
	float4 camera_position;
	float4 light_position;
	float light_radius;
	float time;
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
	float f = (f0 + (1.0f - f0) * pow(2.0f, (-5.55473 * voh - 6.98316) * voh));
	return f;
}
float D_GGX(float a, float noh)
{
	float d = a / (noh * noh * (a * a - 1.0f) + 1.0f);
	return d * d;
}
float3 specGGX(float3 n, float3 l, float3 v, float roughness, float spec)
{
	//Cook-Torrance GGX Shading
	float ks = spec;
	float3 halfway = normalize(v + l);
	float3 is = float3(1, 1, 1);
	float3 specular = ks * is * max(dot(l, n), 0) * D_GGX(roughness * roughness, dot(n, halfway)) * F_GGX(0.9f, dot(l, halfway)) * G_GGX(roughness, n, l, v);
	return specular;
}

float3 specPhong(float3 n, float3 l, float3 v, float glossy)
{
	//Phong Shading
	float ks = 1.0f;
	float3 is = float3(1, 1, 1);
	float3 specular = ks * pow(max(dot(reflect(l, n), v), 0), glossy) * is;
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

float shadowCalc(float4 pixelPosLightSpace)
{
	float3 coords = pixelPosLightSpace.xyz / pixelPosLightSpace.w;
	coords = coords * 0.5f + 0.5f;
	float depth = shadowMap.Sample(ShadowMapSampler, coords.xy).r;
	float currentDepth = coords.z;
	return currentDepth > depth ? 1.0 : 0.0;
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float ka = 1.0f;
	float3 ia = float3(0.09, 0.082, 0.082);
	float3 ambient = ka * ia;

	float kd = 1.0f;
	float3 id = tex.Sample(TextureSampler, 1.0f - input.texcoord);
	float3 halfway = normalize(-input.camera_direction + light_direction.xyz);
	float3 diffuse = kd * id * max(dot(light_direction.xyz, input.normal), 0);
	// * max(dot(light_direction.xyz, input.normal), 0)
	// * (1.0f - F_GGX(0.1f, dot(light_direction.xyz, halfway)))

	//float3 specular = specGGX(input.normal, light_direction.xyz, -input.camera_direction, 0.25f, 1.0f);
	//float3 specular = specPhong(input.normal, -light_direction.xyz, -input.camera_direction, 20.0f);
	float3 specular = specBlinnPhong(input.normal, light_direction.xyz, -input.camera_direction, 20.0f);

	float shadow = shadowCalc(input.pixelPosLightSpace);

	float3 final_color = ambient + (1.0f - shadow) * (diffuse + specular);

	return float4(final_color, 1.0);
}