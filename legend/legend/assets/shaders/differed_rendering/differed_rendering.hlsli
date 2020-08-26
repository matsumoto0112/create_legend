#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSLI
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSLI

/**
 * @file differed_rendering.hlsli
 * @brief
 */

#include "../defines/global.hlsli"

struct VSInput {
    float3 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

typedef VSOutput PSInput;

struct LightState {
    float4x4 view;
    float4x4 proj;
    float3 position;
    float pad0;
    float3 direction;
    float pad1;
    float4 color;
};
ConstantBuffer<LightState> g_light_cb: register(b7);


#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSLI
