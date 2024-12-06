TextureCube myTexture : register(t0);
SamplerState samplerState : register(s0);

struct PSOutput
{
    float4 Diffuse : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Plane : SV_Target3;
};

PSOutput main(float4 pos : SV_POSITION, float3 direction : TEXCOORD) : SV_Target
{
    PSOutput output;

    output.Diffuse = myTexture.Sample(samplerState, normalize(direction));
    output.Normal = 0;
    output.Position = 0;

    return output;
}
