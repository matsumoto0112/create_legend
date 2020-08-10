#ifndef LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_CS_HLSL_
#define LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_CS_HLSL_

/**
* @file gpu_particle_cs.hlsl
*/

#define THREAD_X 1
#define THREAD_Y 1

#define DISPATCH_X 1
#define DISPATCH_Y 1


struct CSInput
{
    uint3 group_thread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint group_index : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

struct Particle
{
    float3 position;
};

RWByteAddressBuffer particles : register(u0);

[numthreads(THREAD_X, THREAD_Y, 1)]
void main(const CSInput input)
{
    const uint index = input.dispatch.z * DISPATCH_X * THREAD_X * DISPATCH_Y * THREAD_Y
        + input.dispatch.y * DISPATCH_X * THREAD_X
        + input.dispatch.x;

    const uint addr = index;

    float3 pos = asfloat(particles.Load3(addr));
    pos += float3(0.0, 0.001, 0.0);
    particles.Store3(addr, asuint(pos));
}

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_CS_HLSL_
