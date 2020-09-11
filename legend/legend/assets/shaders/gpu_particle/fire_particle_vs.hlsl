#ifndef LEGEND_ASSETS_SHADERS_GPU_PARTICLE_FIRE_PARTICLE_VS_HLSL
#define LEGEND_ASSETS_SHADERS_GPU_PARTICLE_FIRE_PARTICLE_VS_HLSL

/**
 * @file fire_particle_vs.hlsl
 * @brief 炎パーティクル描画用頂点シェーダー
 */

#ifndef HLSL
#define HLSL
#endif

#include "fire_particle.h"

GSInput main(VSInput input) {
  GSInput result = (GSInput)0;

  result.position = float4(input.position, 0.0f);
  result.scale = input.scale;
  result.color = input.color;
  return result;
}
#endif  //! LEGEND_ASSETS_SHADERS_GPU_PARTICLE_FIRE_PARTICLE_VS_HLSL
