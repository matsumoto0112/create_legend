#ifndef LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_PS_HLSL
#define LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_PS_HLSL

/**
 * @file player_move_particle_ps.hlsl
 * @brief プレイヤー移動時パーティクル描画用ピクセルシェーダー
 */

#ifndef HLSL
#define HLSL
#endif

#include "player_move_particle.h"

float4 main(const PSInput input) : SV_TARGET {
  float4 color = g_albedo.Sample(g_sampler_warp, input.uv);
  color *= input.color;
  return color;
}

#endif  //! LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_PS_HLSL
