#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PS_HLSL
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PS_HLSL

/**
 * @file differed_rendering_ps.hlsl
 * @brief
 */

#include "differed_rendering.hlsli"

texture2D g_world_position : register(t0);
texture2D g_world_normal : register(t1);
texture2D g_diffuse : register(t2);

float4 main(const PSInput i) : SV_TARGET{
    //return g_diffuse.Sample(g_sampler_warp, i.uv);
    return float4(1,1,1,1);
}

#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PS_HLSL
