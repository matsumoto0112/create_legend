#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_PS_HLSL
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_PS_HLSL

/**
 * @file differed_rendering_pre_ps.hlsl
 * @brief
 */

#include "differed_rendering_pre.hlsli"

PSOutput main(const PSInput i) {
    PSOutput result = (PSOutput)0;
    float depth = i.world_pos.z / i.world_pos.w;

    result.position = float4(i.world_pos.xyz, 1.0);
    result.normal = float4(i.normal.xyz * 0.5 + 0.5, 1.0);
    result.diffuse = g_albedo.Sample(g_sampler_warp, i.uv);
    return result;
}

#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_PS_HLSL
