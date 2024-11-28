Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D PositionTexture : register(t2);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float4 diffuse = DiffuseTexture.Sample(samplerState, input.TexCoord);
    float3 normal = NormalTexture.Sample(samplerState, input.TexCoord).xyz;
    float3 worldPos = PositionTexture.Sample(samplerState, input.TexCoord).xyz;

    return diffuse;
}
