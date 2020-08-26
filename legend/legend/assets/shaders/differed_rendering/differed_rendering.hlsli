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
    float4 light_pos : POSITION;
};

typedef VSOutput PSInput;


#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSLI
