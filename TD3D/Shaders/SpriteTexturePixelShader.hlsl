struct PSOutput
{
    float4 Diffuse : SV_Target;
};

Texture2D myTexture : register(t0);
SamplerState samplerState : register(s0);

PSOutput main(float4 pos : SV_Position, float2 uv : TEXCOORD) : SV_Target
{
    PSOutput output;
    output.Diffuse =  myTexture.Sample(samplerState, uv);
    return output;
}
