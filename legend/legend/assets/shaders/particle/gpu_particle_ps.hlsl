#ifndef LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_PS_HLSL_
#define LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_PS_HLSL_

/**
* @file gpu_particle_ps.hlsl
*/

struct PSInput
{
    float4 pos : SV_POSITION;
};

float4 main(const PSInput input) : SV_TARGET
{
    return float4(1.0, 0.0, 0.5, 1.0);
}

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_PS_HLSL_
