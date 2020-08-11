#ifndef LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_PS_HLSL_
#define LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_PS_HLSL_

/**
* @file gpu_particle_ps.hlsl
*/

#include "gpu_particle_test.hlsli"

float4 main(const PSInput input) : SV_TARGET
{
    return input.color;
}

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_PS_HLSL_
