struct PSOutput
{
    float4 Diffuse : SV_Target;
};

PSOutput main(float4 pos : SV_Position, float4 color : COLOR) : SV_Target
{
    PSOutput output;
    output.Diffuse = color;
    return output;
}
