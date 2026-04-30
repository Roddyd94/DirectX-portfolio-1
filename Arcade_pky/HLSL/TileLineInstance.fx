#include "Share.fx"

struct TileLineInstanceData
{
    float4 color;
    float2 worldPos;
    float2 size;
};

StructuredBuffer<TileLineInstanceData> tileLineInstances : register(t1);

float4 TileLineInstanceVS(float3 position : POSITION, uint instanceID : SV_InstanceID) : SV_POSITION
{
    float4 output = (float4) 0;

    TileLineInstanceData instance = tileLineInstances[instanceID];
    
    float3 pos = position;
    pos.x = pos.x * instance.size.x + instance.worldPos.x;
    pos.y = pos.y * instance.size.y + instance.worldPos.y;
    pos.z = 1.f;
    
    output = mul(float4(pos, 1.f), gWvp);
    
    return output;
}

PS_Output_Single TileLineInstancePS(float4 input : SV_Position)
{
    PS_Output_Single output = (PS_Output_Single) 0;

    output.color = gFrameLineColor;
    
    return output;
}