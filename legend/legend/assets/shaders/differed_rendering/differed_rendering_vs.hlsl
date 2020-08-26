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

    return result;
}

#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSL
