#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_VS_HLSL
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_VS_HLSL

/**
 * @file differed_rendering_pre_vs.hlsl
 * @brief
 */

#include "differed_rendering_pre.hlsli"

VSOutput main(const VSInput v) {
    VSOutput result = (VSOutput)0;
    result.world_pos = mul(float4(v.position, 1.0), g_transform.world);
    result.position = mul(result.world_pos, g_world_context.view);
    result.position = mul(result.position, g_world_context.projection);

    result.normal = mul(v.normal, (float3x3)g_transform.world);
    result.uv = v.uv;

    return result;
}


#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_VS_HLSL
