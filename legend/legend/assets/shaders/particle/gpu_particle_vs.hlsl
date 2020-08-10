#ifndef LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_VS_HLSL_
#define LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_VS_HLSL_

/**
* @file gpu_particle_vs.hlsl
*/

struct VSInput
{
    float3 position : POSITION; //!< À•W
};

struct GSInput
{
    float4 position : SV_POSITION;
};


GSInput main(VSInput input)
{
    GSInput result = (GSInput)0;

    result.position = float4(input.position, 1.0f);
    return result;
}

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_VS_HLSL_
