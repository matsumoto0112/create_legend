#ifndef LEGEND_ASSETS_SHADERS_POSTPROCESS_GRAY_SCALE_HLSLI_
#define LEGEND_ASSETS_SHADERS_POSTPROCESS_GRAY_SCALE_HLSLI_

#include "../defines/global.hlsli"

struct VSInput{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOutput{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

typedef VSOutput PSInput;

VSOutput VSMain(const VSInput v){
    VSOutput o = (VSOutput)0;
    o.pos = mul(float4(v.pos,1.0),g_transform.world);
    o.pos = mul(o.pos,g_world_context.view);
    o.pos = mul(o.pos,g_world_context.projection);
    
    o.uv = v.uv;
    
    return o;
}

float4 PSMain(const PSInput i){
    float4 color = g_albedo.Sample(g_sampler_linear,i.uv);
    float gray = color.r * 0.2125+ color.g * 0.7154+ color.b * 0.0721;
    color = float4(gray,gray,gray,1.0);
    return color;
}

#endif //! LEGEND_ASSETS_SHADERS_POSTPROCESS_GRAY_SCALE_HLSLI_
