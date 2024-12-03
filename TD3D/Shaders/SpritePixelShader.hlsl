struct PSOutput
{
    float4 Diffuse : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Plane : SV_Target3;
};

Texture2D myTexture : register(t0);
SamplerState samplerState : register(s0);

PSOutput main(float4 pos : SV_Position, float2 uv : TEXCOORD) : SV_Target
{
    PSOutput output;
    output.Plane = myTexture.Sample(samplerState, uv);
    return output;
}
