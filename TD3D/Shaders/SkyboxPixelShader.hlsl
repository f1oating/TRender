TextureCube myTexture : register(t0);
SamplerState samplerState : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_Target
{
    return myTexture.Sample(samplerState, float3(uv, 1.0f));
}
