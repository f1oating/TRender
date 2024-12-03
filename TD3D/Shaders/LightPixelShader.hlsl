struct Light
{
    int Type;
    float3 Position;
    float3 Direction;
    float3 Color;
    float Intensity;
    float Range;
    float SpotAngle;
};

StructuredBuffer<Light> Lights : register(t4);
cbuffer LightInfo : register(b0)
{
    int LightCount;
    int align[3];
};

float3 CalculateDirectionalLight(Light light, float3 normal)
{
    float3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal, lightDir), 0.0);
    return light.Color * diff * light.Intensity;
}

float3 CalculatePointLight(Light light, float3 worldPos, float3 normal)
{
    float3 lightDir = light.Position - worldPos;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    float diff = max(dot(normal, lightDir), 0.0);
    float attenuation = 1.0 / (distance * distance + 1e-4);

    if (distance < light.Range)
    {
        return light.Color * diff * attenuation * light.Intensity;
    }
    return float3(0, 0, 0);
}

float3 CalculateSpotLight(Light light, float3 worldPos, float3 normal)
{
    float3 lightDir = light.Position - worldPos;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    float theta = dot(lightDir, normalize(-light.Direction));
    float epsilon = cos(radians(light.SpotAngle) * 0.5);

    if (theta > epsilon && distance < light.Range)
    {
        float diff = max(dot(normal, lightDir), 0.0);
        float attenuation = 1.0 / (distance * distance + 1e-4);
        float spotEffect = smoothstep(epsilon, 1.0, theta);

        return light.Color * diff * attenuation * spotEffect * light.Intensity;
    }
    return float3(0, 0, 0);
}

Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D PositionTexture : register(t2);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float4 diffuse = DiffuseTexture.Sample(samplerState, input.TexCoord);
    float3 normal = normalize(NormalTexture.Sample(samplerState, input.TexCoord).xyz * 2.0 - 1.0);
    float3 worldPos = PositionTexture.Sample(samplerState, input.TexCoord).xyz;

    float3 finalColor = float3(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < LightCount; ++i)
    {
        Light light = Lights[i];
        if (light.Type == 0)
        {
            finalColor += CalculateDirectionalLight(light, normal);
        }
        else if (light.Type == 1)
        {
            finalColor += CalculatePointLight(light, worldPos, normal);
        }
        else if (light.Type == 2)
        {
            finalColor += CalculateSpotLight(light, worldPos, normal);
        }
    }

    return float4(finalColor * diffuse.rgb, diffuse.a);
}
