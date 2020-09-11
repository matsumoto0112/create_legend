#ifndef LEGEND_ASSETS_SHADERS_BULLET_BULLET_DEBUG_DRAW_VS_HLSL
#define LEGEND_ASSETS_SHADERS_BULLET_BULLET_DEBUG_DRAW_VS_HLSL

/**
 * @file bullet_debug_draw_vs.hlsl
 * @brief bulletデバッグ描画用頂点シェーダー
 */

#include "bullet_debug_draw.hlsli"

VSOutput main(const VSInput v) {
  VSOutput result = (VSOutput)0;
  result.pos = float4(v.pos.xyz, 1.0);
  result.pos = mul(result.pos, g_world_context.view);
  result.pos = mul(result.pos, g_world_context.projection);

  result.color = v.color;
  return result;
}

#endif  //! LEGEND_ASSETS_SHADERS_BULLET_BULLET_DEBUG_DRAW_VS_HLSL
