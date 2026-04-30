#include "Share.fx"

struct TileInstanceData
{
    float2 uvLT;
    float2 uvRB;
    float2 worldPos;
    float2 size;
};

StructuredBuffer<TileInstanceData> tileInstances : register(t1);

struct VS_Input
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_Output TileInstanceVS(VS_Input input, uint instanceID : SV_InstanceID)
{
    VS_Output output = (VS_Output) 0;

    TileInstanceData instance = tileInstances[instanceID];
    
    float3 pos = input.pos;
    pos.x = pos.x * instance.size.x + instance.worldPos.x;
    pos.y = pos.y * instance.size.y + instance.worldPos.y;
    pos.z = 1.f;
    
    output.pos = mul(float4(pos, 1.f), gWvp);
    
    output.uv = UpdateSpriteAtlasUV(input.uv, instance.uvLT, instance.uvRB);
    
    return output;
}

PS_Output_Single TileInstancePS(VS_Output input)
{
    PS_Output_Single output = (PS_Output_Single) 0;

    output.color = gBaseTexture.Sample(gBaseSampler, input.uv);
    
    return output;
}