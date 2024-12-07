cbuffer WorldMatrix : register(b2)
{
    float4x4 world;
};

cbuffer LightMatrix : register(b3)
{
    float4x4 lightViewProjection;
};

struct VS_INPUT
{
    float3 position : POSITION;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 worldPos = mul(float4(input.position, 1.0f), world);

    output.position = mul(worldPos, lightViewProjection);

    return output;
}
