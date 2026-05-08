#include "Share.fx"

struct VS_Output
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

struct TileOutlineInstanceData
{
    float4 color;
    float2 worldPos;
    float2 size;
};

StructuredBuffer<TileOutlineInstanceData> tileOutlineInstances : register(t1);

VS_Output TileOutlineInstanceVS(float3 position : POSITION, uint instanceID : SV_InstanceID)
{
    VS_Output output = (VS_Output) 0;

    TileOutlineInstanceData instance = tileOutlineInstances[instanceID];
    
    float3 pos = position;
    pos.x = pos.x * instance.size.x + instance.worldPos.x;
    pos.y = pos.y * instance.size.y + instance.worldPos.y;
    pos.z = 1.f;
    
    output.pos =  mul(float4(pos, 1.f), gWvp);
    output.color = instance.color;
    
    return output;
}

PS_Output_Single TileOutlineInstancePS(VS_Output input)
{
    PS_Output_Single output = (PS_Output_Single) 0;

    output.color = input.color;
    
    return output;
}