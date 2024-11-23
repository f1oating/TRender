struct VSOut
{
	float4 pos : SV_Position;
	float4 color : COLOR;
};

VSOut main(float2 pos : POSITION, float4 color : COLOR)
{
	VSOut vso;
	vso.pos = float4(pos, 0.0f, 1.0f);
	vso.color = color;
	return vso;
}