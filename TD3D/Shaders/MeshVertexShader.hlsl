cbuffer ProjectionBuffer : register(b0)
{
	matrix projectionMatrix;
};

cbuffer ViewBuffer : register(b1)
{
	matrix viewMatrix;
};

cbuffer WorldBuffer : register(b2)
{
	matrix worldMatrix;
};

struct VSOut
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

VSOut main(float3 pos : POSITION, float2 uv : TEXCOORD, float3 normal : NORMAL, float3 tangent : TANGENT)
{
	VSOut vso;
	vso.pos = mul(float4(pos, 1.0f), mul(mul(viewMatrix, projectionMatrix), worldMatrix));
	vso.uv = uv;
	return vso;
}