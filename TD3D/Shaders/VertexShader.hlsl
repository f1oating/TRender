cbuffer CBuf : register(b0)
{
	matrix transform;
};

struct VSOut
{
	float4 color : COLOR;
	float3 normal : NORMAL;
	float3 worldPos : POSITION;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION, float4 color : COLOR, float3 normal : NORMAL)
{
	VSOut vso;
	vso.pos = mul(float4(pos, 1.0f), transform);
	vso.worldPos = vso.pos;
	vso.color = color;
	vso.normal = normal;
	return vso;
}