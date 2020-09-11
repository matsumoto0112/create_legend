#ifndef LEGEND_ASSETS_SHADERS_GPU_PARTICLE_EXPLOSION_H
#define LEGEND_ASSETS_SHADERS_GPU_PARTICLE_EXPLOSION_H

/**
 * @file explosion.h
 * @brief 爆発時のパーティクル情報定義
 */

#include "gpu_particle.h"

#ifndef HLSL
namespace legend {
namespace shaders {
namespace gpu_particle {
namespace explosion {
#endif  //! HLSL

static const uint THREAD_X = 8;
static const uint THREAD_Y = 1;
static const uint DISPATCH_X = 1;
static const uint DISPATCH_Y = 1;

struct Particle {
  uint seed;
  float lifetime;
  Vector3 velocity;
  Vector3 position;
  Vector3 scale;
  Color4 color;
};

#ifdef HLSL
struct VSInput {
  uint seed : IN_SEED;
  float lifetime : IN_LIFETIME;
  float3 velocity : IN_VELOCITY;
  float3 position : POSITION;
  float3 scale : SCALE;
  float4 color : COLOR;
};

struct VSOutput {
  float4 position : SV_POSITION;
  float3 scale : SCALE;
  float4 color : COLOR;
};

typedef VSOutput GSInput;

struct GSOutput {
  float4 position : SV_POSITION;
  float2 uv : TEXCOORD0;
  float4 color : COLOR;
};

typedef GSOutput PSInput;
#else
}  // namespace player_move_particle
}  // namespace gpu_particle
}  // namespace shader
}  // namespace legend
#endif

#endif  //! LEGEND_ASSETS_SHADERS_GPU_PARTICLE_EXPLOSION_H
