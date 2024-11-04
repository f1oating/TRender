struct Light {
    int type;

    float3 Position;
    float Range;
    float3 Color;
    float Intensity;
    float3 Direction;
    float SpotAngle;

    float3 lightPadding;
};

struct AmbientLight
{
    float4 Color;
};

struct DirectLight
{
    float3 Direction;
    float Intensity;
    float3 Color;
    float Pad;
};

cbuffer AmbientLightBuffer : register(b0)
{
    AmbientLight ambientLight;
};

cbuffer LightBuffer : register(b1) {
    Light lights[6];
    int numLights;
    float3 padding;
};

cbuffer DirectLightBuffer : register (b2)
{
    DirectLight directLight;
}

float3 CalculatePointLight(Light light, float3 normal, float3 worldPos)
{
    float3 lightDir = light.Position - worldPos;
    float distance = length(lightDir);

    if (distance >= light.Range)
    {
        return float3(0, 0, 0);
    }

    lightDir = normalize(lightDir);

    float diff = max(dot(normal, lightDir), 0.0f);

    float attenuation = saturate(1.0f - (distance / light.Range));

    return diff * light.Color * light.Intensity * attenuation;
}

float4 CalculateSpotLight(Light light, float3 normal, float3 fragPos) {
    float3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal, -lightDir), 0.0);

    float distance = length(light.Position - fragPos);
    if (distance > light.Range) return float4(0, 0, 0, 1);

    float theta = dot(lightDir, normalize(-light.Direction));
    float epsilon = light.SpotAngle;
    float intensity = saturate((theta - epsilon) / (1.0 - epsilon));

    return float4(light.Color * diff * intensity * light.Intensity, 1.0);
}

float4 CalculateDirectLight(float3 normal)
{
    float3 lightDir = normalize(directLight.Direction);

    float diff = max(dot(normal, -lightDir), 0.0f);

    return float4(directLight.Color * diff * directLight.Intensity, 1.0f);
}

float4 main(float4 color : COLOR, float3 normal : NORMAL, float3 worldPos : POSITION) : SV_Target
{
    float3 finalColor = color.rgb * ambientLight.Color.rgb;
    finalColor += CalculateDirectLight(normal);

    for (int i = 0; i < numLights; ++i)
    {
        if (lights[i].type == 0)
        {
            finalColor += CalculatePointLight(lights[i], normal, worldPos);
        }
        else
        {
            finalColor += CalculateSpotLight(lights[i], normal, worldPos);
        }
    }

    return float4(finalColor, color.a);
}
