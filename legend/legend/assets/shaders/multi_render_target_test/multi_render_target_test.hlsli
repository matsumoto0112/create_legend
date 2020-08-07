#ifndef LEGEND_ASSETS_SHADERS_MULTI_RENDER_TARGET_TEST_MULTI_RENDER_TARGET_TEST_HLSLI_
#define LEGEND_ASSETS_SHADERS_MULTI_RENDER_TARGET_TEST_MULTI_RENDER_TARGET_TEST_HLSLI_

#include "../defines/global.hlsli"

struct VSInput {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 tangent : TANGENT;
};

struct VSOutput {
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

typedef VSOutput PSInput;

struct PSOutput {
    float4 output_1 : SV_TARGET0;    
    float4 output_2 : SV_TARGET1;    
};

VSOutput VSMain(const VSInput v) {
    VSOutput o = (VSOutput)0;
    o.pos = mul(float4(v.pos,1.0),g_transform.world);
    o.pos = mul(o.pos,g_world_context.view);
    o.pos = mul(o.pos,g_world_context.projection);
    
    o.normal = mul(v.normal,(float3x3)g_transform.world);
    o.normal= v.normal;
    o.uv = v.uv;
    
    return o;
}

PSOutput PSMain(const PSInput i) {
    PSOutput o = (PSOutput)0;
    o.output_1 = g_albedo.Sample(g_sampler_warp, i.uv);
    o.output_2 = g_albedo.Sample(g_sampler_clamp, i.uv);
    return o;
}

#endif //! LEGEND_ASSETS_SHADERS_MULTI_RENDER_TARGET_TEST_MULTI_RENDER_TARGET_TEST_HLSLI_
