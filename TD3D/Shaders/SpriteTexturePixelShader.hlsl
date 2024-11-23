Texture2D myTexture : register(t0);
SamplerState samplerState : register(s0);

float4 main(float4 pos : SV_Position, float2 uv : TEXCOORD) : SV_Target
{
    return myTexture.Sample(samplerState, uv);
}
