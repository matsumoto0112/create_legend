#ifndef LEGEND_ASSETS_SHADERS_MODELVIEW_MODEL_VIEW_HLSLI_
#define LEGEND_ASSETS_SHADERS_MODELVIEW_MODEL_VIEW_HLSLI_

#include "../defines/global.h"

struct VSInput{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

struct VSOutput{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

typedef VSOutput PSInput;

VSOutput VSMain(const VSInput v){
    VSOutput o = (VSOutput)0;
    o.pos = mul(float4(v.pos,1.0),g_transform.world);
    o.pos = mul(o.pos,g_world_context.view);
    o.pos = mul(o.pos,g_world_context.projection);
    
    o.normal = mul(v.normal,(float3x3)g_transform.world);
    o.normal= v.normal;
    o.uv = v.uv;
    
    return o;
}

float4 PSMain(const PSInput i){
    //float3 color = i.normal * 0.5 + 0.5;
    //float4 color = float4(i.uv,0.0,0.0);
    float4 color = g_albedo.Sample(g_sampler_linear,i.uv);
    return color;
}

#endif //! LEGEND_ASSETS_SHADERS_MODELVIEW_MODEL_VIEW_HLSLI_
