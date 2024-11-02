cbuffer AmbientLightBuffer : register(b0)
{
    float4 ambientColor;
};

struct PointLight {
    float3 Position;
    float Range;
    float3 Color;
    float Intensity;
};

cbuffer LightBuffer : register(b1) {
    PointLight pointLights[8];
    int numPointLights;
};

float4 main(float4 color : COLOR, float3 normal : NORMAL, float3 worldPos : POSITION) : SV_Target
{
    float3 finalColor = color.rgb * ambientColor.rgb;

    for (int i = 0; i < numPointLights; ++i)
    {
        float3 lightDir = pointLights[i].Position - worldPos;
        float distance = length(lightDir);

        if (distance < pointLights[i].Range)
        {
            lightDir = normalize(lightDir);

            float diff = max(dot(normalize(normal), lightDir), 0.0f);

            float attenuation = saturate(1.0f - (distance / pointLights[i].Range));
            finalColor += diff * pointLights[i].Color * pointLights[i].Intensity * attenuation;
        }
    }

    return float4(finalColor, color.a);
}
