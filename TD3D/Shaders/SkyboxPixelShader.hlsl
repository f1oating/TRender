TextureCube myTexture : register(t0);
SamplerState samplerState : register(s0);

float4 main(float4 pos : SV_POSITION, float3 direction : TEXCOORD) : SV_Target
{
    return myTexture.Sample(samplerState, normalize(direction));
}
