#ifndef LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_TEST_HLSLI_
#define LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_TEST_HLSLI_

/**
* @file gpu_particle_test.hlsli
*/

#ifndef HLSL
#define HLSL
#endif //! HLSL

#include "../defines/global.hlsli"
#include "gpu_particle_test.h"


struct VSInput
{
    uint seed : IN_SEED;
    float lifetime : IN_LIFETIME;
    float3 velocity : IN_VELOCITY;
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

typedef VSOutput GSInput;

struct GSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR;
};

typedef GSOutput PSInput;

ConstantBuffer<ParticleInfo> g_particle_info : register(b5);

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_TEST_HLSLI_
