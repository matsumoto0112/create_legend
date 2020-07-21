#ifndef LEGEND_ASSETS_SHADERS_MULTI_RENDER_TARGET_TEST_MULTI_RENDER_TARGET_TEST_PP_HLSLI_
#define LEGEND_ASSETS_SHADERS_MULTI_RENDER_TARGET_TEST_MULTI_RENDER_TARGET_TEST_PP_HLSLI_

#include "../defines/global.hlsli"

Texture2D<float4> g_output1 : register(t3);
Texture2D<float4> g_output2 : register(t5);

struct MultiRenderTargetTestPP_ConstantBufferStructure {
    float border;
};
ConstantBuffer<MultiRenderTargetTestPP_ConstantBufferStructure> g_local_cb : register(b2);

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
    float4 color;
    if(i.uv.x <= g_local_cb.border){
     color =    g_output1.Sample(g_sampler_linear,i.uv);
    }
    else{
         color =    g_output2.Sample(g_sampler_linear,i.uv);
    }
    //float gray = color.r * 0.2125+ color.g * 0.7154+ color.b * 0.0721;
    //color = float4(gray,gray,gray,1.0);
    return color;
}

#endif //! LEGEND_ASSETS_SHADERS_MULTI_RENDER_TARGET_TEST_MULTI_RENDER_TARGET_TEST_PP_HLSLI_
