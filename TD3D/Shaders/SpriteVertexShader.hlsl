struct VSOut
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

VSOut main(float2 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOut vso;
	vso.pos = float4(pos, 0.0f, 1.0f);
	vso.uv = uv;
	return vso;
}