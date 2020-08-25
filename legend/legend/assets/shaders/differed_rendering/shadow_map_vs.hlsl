#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_CREATE_SHADOW_MAP_VS_HLSL
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_CREATE_SHADOW_MAP_VS_HLSL

/**
 * @file create_shadow_map_vs.hlsl
 * @brief
 */

#include "../defines/global.hlsli"

struct VSInput {
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 tangent : TANGENT;
};

struct VSOutput {
    float4 position : SV_POSITION;
};

VSOutput main(const VSInput v) {
    VSOutput result = (VSOutput)0;
    float4 world_pos = mul(float4(v.position, 1.0), g_transform.world);
    float4 view_pos = mul(world_pos, g_world_context.view);
    float4 proj_pos = mul(view_pos, g_world_context.projection);

    result.position = proj_pos;
    return result;
}

#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_CREATE_SHADOW_MAP_VS_HLSL
