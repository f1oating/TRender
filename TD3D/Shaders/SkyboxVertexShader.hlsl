cbuffer CBuf : register(b0)
{
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VSOut
{
	float4 pos : SV_Position;
	float3 direction : TEXCOORD;
};

VSOut main(float3 pos : POSITION, float3 direction : TEXCOORD)
{
	VSOut vso;
	matrix viewMatrixNoTranslation = viewMatrix;
	viewMatrixNoTranslation._41 = 0.0f;
	viewMatrixNoTranslation._42 = 0.0f;
	viewMatrixNoTranslation._43 = 0.0f;
	vso.pos = mul(float4(pos, 1.0f), mul(viewMatrixNoTranslation, projectionMatrix));
	vso.direction = direction;
	return vso;
}
