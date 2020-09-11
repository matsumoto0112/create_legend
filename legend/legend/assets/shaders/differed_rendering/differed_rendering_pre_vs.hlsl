#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_VS_HLSL
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_VS_HLSL

/**
 * @file differed_rendering_pre_vs.hlsl
 * @brief Differed-Rendering事前描画用頂点シェーダー定義
 */

#include "differed_rendering_pre.hlsli"

VSOutput main(const VSInput v) {
  VSOutput result = (VSOutput)0;

  float4 world_pos = mul(float4(v.position, 1.0), g_transform.world);
  float4 view_pos = mul(world_pos, g_world_context.view);
  float4 proj_pos = mul(view_pos, g_world_context.projection);

  result.position = proj_pos;
  result.world_pos = world_pos;
  result.normal = mul(v.normal, (float3x3)g_transform.world);
  result.uv = v.uv;

  return result;
}

#endif  //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_VS_HLSL
