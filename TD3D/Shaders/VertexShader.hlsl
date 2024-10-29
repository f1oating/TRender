struct VSOut
{
	float4 color : COLOR;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION, float4 color : COLOR)
{
	VSOut vso;
	vso.pos = float4(pos.x, pos.y, pos.z, 1.0f);
	vso.color = color;
	return vso;
}