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

struct CSInput {
    uint3 group_thread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint group_index : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

#define LIFETIME_START (4 * 0)
#define POSITION_START (4 * 1)
#define COLOR_START (4 * 4)
#define PARTICLE_SIZE (4 * 8)

RWByteAddressBuffer particles : register(u0);

struct VSInput
{
    float lifetime : IN_LIFETIME;
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
    float4 color : COLOR;
};

typedef GSOutput PSInput;

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_TEST_HLSLI_
