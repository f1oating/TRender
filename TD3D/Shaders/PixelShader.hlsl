cbuffer AmbientLightBuffer : register(b0)
{
	float4 ambientColor;
};

float4 main(float4 color : COLOR) : SV_Target
{
	return color * ambientColor;
}