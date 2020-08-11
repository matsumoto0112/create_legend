#ifndef LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_VS_HLSL_
#define LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_VS_HLSL_

/**
* @file gpu_particle_vs.hlsl
*/

#include "gpu_particle_test.hlsli"

GSInput main(VSInput input)
{
    GSInput result = (GSInput)0;

    result.position = float4(input.position, 1.0f);
    result.color = input.color;
    return result;
}

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_VS_HLSL_
