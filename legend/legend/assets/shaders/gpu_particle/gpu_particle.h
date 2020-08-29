#ifndef LEGEND_ASSETS_SHADERS_GPU_PARTICLE_GPU_PARTICLE_H
#define LEGEND_ASSETS_SHADERS_GPU_PARTICLE_GPU_PARTICLE_H

/**
 * @file gpu_particle.h
 * @brief
 */

#ifdef HLSL
typedef float2 Vector2;
typedef float3 Vector3;
typedef float4 Vector4;
typedef float4 Color4;
#else
#include "src/math/vector_2.h"
#include "src/math/vector_3.h"
#include "src/math/vector_4.h"
#include "src/util/color_4.h"

using uint = legend::u32;
using Vector2 = legend::math::Vector2;
using Vector3 = legend::math::Vector3;
using Vector4 = legend::math::Vector4;
using Color4 = legend::util::Color4;

namespace legend {
namespace shader {
namespace gpu_particle {
#endif  //! HLSL

struct ParticleInfo {
  uint reset;
  uint emit_enable;
};

#ifdef HLSL
#include "../defines/global.hlsli"
struct CSInput {
  uint3 group_thread : SV_GroupThreadID;
  uint3 group : SV_GroupID;
  uint group_index : SV_GroupIndex;
  uint3 dispatch : SV_DispatchThreadID;
};
ConstantBuffer<ParticleInfo> g_particle_info : register(b5);
#else
}  // namespace gpu_particle
}  // namespace shader
}  // namespace legend
#endif

#endif  //! LEGEND_ASSETS_SHADERS_GPU_PARTICLE_GPU_PARTICLE_H
