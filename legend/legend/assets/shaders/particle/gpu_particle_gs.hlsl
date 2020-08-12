#ifndef LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_GS_HLSL_
#define LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_GS_HLSL_

/**
* @file gpu_particle_gs.hlsl
*/

#include "gpu_particle_test.hlsli"

#define MAX_VERTEX_COUNT 6
[maxvertexcount(MAX_VERTEX_COUNT)]
void main(point GSInput input[1], inout TriangleStream<GSOutput> output)
{
    const float4x4 m = mul(g_world_context.view, g_world_context.projection);

    const float4 pos[4] =
    {
        float4(-0.5f, 0.5f, 0.0f, 1.0f),
        float4(0.5f, 0.5f, 0.0f, 1.0f),
        float4(0.5f, -0.5f, 0.0f, 1.0f),
        float4(-0.5f, -0.5f, 0.0f, 1.0f),
    };

    const float2 uv[4] = {
        float2(0.0, 0.0),
        float2(1.0, 0.0),
        float2(1.0, 1.0),
        float2(0.0, 1.0)
    };

    const uint indices[MAX_VERTEX_COUNT] =
    {
        0, 1, 2, 0, 2, 3,
    };

    GSOutput o = (GSOutput)0;

    uint i;
    [unroll]
    for (i = 1; i <= MAX_VERTEX_COUNT; i++)
    {
        const uint index = indices[i - 1];
        o.position = pos[index];
        o.position += input[0].position;
        o.position = mul(o.position, m);
        o.uv = uv[index];
        o.color = input[0].color;
        output.Append(o);
        if (i % 3 == 0)
            output.RestartStrip();
    }
}

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_GS_HLSL_
