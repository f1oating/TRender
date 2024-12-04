struct Output
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
    float3 WorldPos : WORLD_POS;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

[maxvertexcount(2)]
void main(triangle Output input[3], inout LineStream<Output> lineStream)
{
    float3 triangleCenter = (input[0].Position + input[1].Position + input[2].Position) / 3.0f;

    float3 normal = normalize(cross(input[1].Position - input[0].Position, input[2].Position - input[0].Position));

    Output vert1, vert2;

    vert1.Position = float4(triangleCenter, 1.0f);
    vert1.Normal = normal;
    vert1.TexCoord = float2(0.0f, 0.0f);
    vert1.WorldPos = triangleCenter;
    vert1.Tangent = float3(1.0f, 0.0f, 0.0f);

    vert2.Position = float4(triangleCenter + normal * 0.1f, 1.0f);
    vert2.Normal = normal;
    vert2.TexCoord = float2(0.0f, 0.0f);
    vert2.WorldPos = triangleCenter + normal * 0.1f;
    vert2.Tangent = float3(1.0f, 0.0f, 0.0f);

    lineStream.Append(vert1);
    lineStream.Append(vert2);

    lineStream.RestartStrip();
}
