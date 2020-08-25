#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSL
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSL

/**
 * @file differed_rendering.hlsl
 * @brief
 */

#include "differed_rendering.hlsli"

VSOutput main(const VSInput v) {
    VSOutput result = (VSOutput)0;

    result.position = float4(v.position, 1.0);
    result.uv = v.uv;

    float3 light_pos = float3(50, 80, 20);
    result.light_pos = mul(float4(light_pos, 1.0), g_world_context.view);
    result.light_pos = mul(result.light_pos, g_world_context.projection);

    return result;
}

#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSL
