cbuffer AmbientLight : register(b1)
{
    float3 ambientLight;
    float padding;
};

Texture2D myTexture : register(t0);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
    float3 WorldPos : WORLD_POS;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

struct PSOutput
{
    float4 Diffuse : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
};

PSOutput main(PSInput input)
{
    PSOutput output;

    output.Diffuse = myTexture.Sample(samplerState, input.TexCoord) * float4(ambientLight, 1.0f);

    output.Normal = float4(normalize(input.Normal), 1.0);

    output.Position = float4(input.WorldPos, 1.0);

    return output;
}
