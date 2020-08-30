#ifndef LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_VS_HLSL
#define LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_VS_HLSL

/**
 * @file player_move_particle_vs.hlsl
 * @brief
 */

#ifndef HLSL
#define HLSL
#endif

#include "player_move_particle.h"

GSInput main(VSInput input) {
  GSInput result = (GSInput)0;

  result.position = float4(input.position, 0.0f);
  result.color = input.color;
  return result;
}

#endif  //! LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_VS_HLSL
