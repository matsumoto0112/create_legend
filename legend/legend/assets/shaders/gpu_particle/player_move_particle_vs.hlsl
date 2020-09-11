#ifndef LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_VS_HLSL
#define LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_VS_HLSL

/**
 * @file player_move_particle_vs.hlsl
 * @brief プレイヤー移動時パーティクル描画用頂点シェーダー
 */

#ifndef HLSL
#define HLSL
#endif

#include "player_move_particle.h"

GSInput main(VSInput input) {
  GSInput result = (GSInput)0;

  result.position = float4(input.position, 0.0f);
  result.color = input.color;
  if (input.lifetime < 0.0) result.color *= 0;
  return result;
}

#endif  //! LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_VS_HLSL
