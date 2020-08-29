#ifndef LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_TEST_H_
#define LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_TEST_H_

/**
* @file gpu_particle_test.h
*/

#ifdef HLSL
typedef const uint CU32;
#else
#include "src/math/vector_3.h"
#include "src/math/vector_4.h"
using CU32 = const legend::u32;
using uint = legend::u32;
using float3 = legend::math::Vector3;
using float4 = legend::math::Vector4;

namespace legend {
    namespace shader {
        namespace gpu_particle {
#endif //! HLSL

            static CU32 THREAD_X = 64;
            static CU32 THREAD_Y = 1;
            static CU32 DISPATCH_X = 1;
            static CU32 DISPATCH_Y = 1;

            struct Particle
            {
                uint seed;
                float lifetime;
                float3 velocity;
                float3 position;
                float4 color;
            };

            struct ParticleInfo {
                uint reset;
                uint emit_enable;
            };

#ifndef HLSL
        }  // namespace gpu_particle
    }  // namespace shader
}  // namespace legend

#endif

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_TEST_H_
