#include "Share.fx"

float4 FrameMeshVS(float3 pos : POSITION) : SV_POSITION
{
    float4 output = (float4) 0;
    
    output = mul(float4(pos, 1.f), gWvp);
    
    return output;
}

PS_Output_Single FrameMeshPS(float4 pos : SV_POSITION)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    output.color = gFrameLineColor;
    
    return output;
}