#ifndef LEGEND_ASSETS_SHADERS_PARTICLE_SIMPLE_PARTICLE_PS_HLSL_
#define LEGEND_ASSETS_SHADERS_PARTICLE_SIMPLE_PARTICLE_PS_HLSL_

#include "simple_particle.hlsli"

float4 main(PS_Input input) : SV_TARGET
{
	return PS_Main(input);
}

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_SIMPLE_PARTICLE_PS_HLSL_