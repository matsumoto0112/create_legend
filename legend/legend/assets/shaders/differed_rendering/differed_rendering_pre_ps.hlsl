#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_PS_HLSL
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_PS_HLSL

/**
 * @file differed_rendering_pre_ps.hlsl
 * @brief
 */

#include "differed_rendering_pre.hlsli"

PSOutput main(const PSInput i) {
    PSOutput result = (PSOutput)0;
    result.position = i.world_pos;
    result.normal = float4(i.normal, 1.0);
    result.diffuse = g_albedo.Sample(g_sampler_warp, i.uv);

    return result;
}

#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_PS_HLSL
