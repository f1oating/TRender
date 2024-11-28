TextureCube myTexture : register(t0);
SamplerState samplerState : register(s0);

struct PSOutput
{
    float4 Diffuse : SV_Target;
};

PSOutput main(float4 pos : SV_POSITION, float3 direction : TEXCOORD) : SV_Target
{
    PSOutput output;

    output.Diffuse = myTexture.Sample(samplerState, normalize(direction));

    return output;
}
