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

struct VSInput
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

struct VSOutput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
    float3 WorldPos : WORLD_POS;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    float4 worldPos = mul(float4(input.Position, 1.0f), worldMatrix);
    float4 viewPos = mul(worldPos, viewMatrix);
    output.Position = mul(viewPos, projectionMatrix);

    output.WorldPos = worldPos.xyz;
    output.Normal = mul((float3x3)worldMatrix, input.Normal);
    output.Tangent = mul((float3x3)worldMatrix, input.Tangent);
    output.TexCoord = input.TexCoord;

    return output;
}
