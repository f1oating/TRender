Texture2D SpriteTexture : register(t3);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float4 diffuse = SpriteTexture.Sample(samplerState, input.TexCoord);
    return diffuse;
}
