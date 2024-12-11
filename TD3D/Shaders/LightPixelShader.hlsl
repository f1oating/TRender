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

struct DirectionalLight
{
    float3 Direction;
    float Intensity;
    float3 Color;
    float Padding;
};

StructuredBuffer<Light> Lights : register(t4);
cbuffer LightInfo : register(b0)
{
    int LightCount;
    int align[3];
};

cbuffer LightMatrix : register(b3)
{
    matrix lightViewProj;
};

cbuffer DirectionalLight : register(b2)
{
    DirectionalLight directionalLight;
};

float3 CalculateDirectionalLight(DirectionalLight directionalLight, float3 normal)
{
    float3 lightDir = normalize(-directionalLight.Direction);
    float diff = max(dot(normal, lightDir), 0.0);
    return directionalLight.Color * diff * directionalLight.Intensity;
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
Texture2D ShadowMap : register(t5);
SamplerState samplerState : register(s0);

struct PSInput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

float ShadowFactor(float3 worldPos)
{
    float4 lightPos = mul(float4(worldPos, 1.0f), lightViewProj);
    lightPos /= lightPos.w;

    float2 shadowUV = lightPos.xy * 0.5f + 0.5f;
    shadowUV.y = 1 - shadowUV.y;

    float shadowDepth = ShadowMap.Sample(samplerState, shadowUV).r;

    float bias = 0.003f;

    return (lightPos.z - bias > shadowDepth) ? 0.0f : 1.0f;
}

float4 main(PSInput input) : SV_Target
{
    float4 diffuse = DiffuseTexture.Sample(samplerState, input.TexCoord);
    float3 normal = normalize(NormalTexture.Sample(samplerState, input.TexCoord).xyz);
    float3 worldPos = PositionTexture.Sample(samplerState, input.TexCoord).xyz;

    float3 finalColor = float3(1.0f, 1.0f, 1.0f);

    finalColor += CalculateDirectionalLight(directionalLight, normal) * ShadowFactor(worldPos);

    for (int i = 0; i < LightCount; ++i)
    {
        Light light = Lights[i];
        if (light.Type == 0)
        {
            finalColor += CalculatePointLight(light, worldPos, normal);
        }
        else if (light.Type == 1)
        {
            finalColor += CalculateSpotLight(light, worldPos, normal);
        }
    }

    return float4(finalColor * diffuse.rgb, diffuse.a);
}
