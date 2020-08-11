#ifndef LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_CS_HLSL_
#define LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_CS_HLSL_

/**
* @file gpu_particle_cs.hlsl
*/

#include "gpu_particle_test.hlsli"

struct CSInput
{
    uint3 group_thread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint group_index : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

#define SEED_START (4 * 0)
#define LIFETIME_START (4 * 1)
#define POSITION_START (4 * 2)
#define COLOR_START (4 * 5)
#define PARTICLE_SIZE (4 * 9)

RWByteAddressBuffer particles : register(u0);

void SetLifeTime(uint addr, float value)
{
    particles.Store(addr + LIFETIME_START, asuint(value));
}

float GetLifeTime(uint addr)
{
    return asfloat(particles.Load(addr + LIFETIME_START));
}

void SetPosition(uint addr, float3 value)
{
    particles.Store3(addr + POSITION_START, asuint(value));
}

float3 GetPosition(uint addr)
{
    return asfloat(particles.Load3(addr + POSITION_START));
}

void SetColor(uint addr, float4 value)
{
    particles.Store4(addr + COLOR_START, asuint(value));
}

float4 GetColor(uint addr)
{
    return asfloat(particles.Load4(addr + COLOR_START));
}

void ResetParticle(uint addr)
{
    float lifetime = 1.0;
    SetLifeTime(addr, lifetime);
    float3 pos = GetPosition(addr);
    pos.y = 0.0;
    SetPosition(addr, pos);
    SetColor(addr, float4(1.0, 0.0, 0.0, 1.0));
}

void UpdateParticle(uint addr)
{
    float lifetime = GetLifeTime(addr);
    lifetime -= 0.01;
    SetLifeTime(addr, lifetime);
    float3 pos = GetPosition(addr);
    pos += float3(0.0, 0.001, 0.0);
    SetPosition(addr, pos);
    float4 color = GetColor(addr);
    color += float4(0.0, 0.0, 0.0, 0.0);
    SetColor(addr, color);
}

[numthreads(THREAD_X, THREAD_Y, 1)]
void main(const CSInput input)
{
    const uint index = input.dispatch.z * DISPATCH_X * THREAD_X * DISPATCH_Y * THREAD_Y
        + input.dispatch.y * DISPATCH_X * THREAD_X
        + input.dispatch.x;

    const uint addr = index * PARTICLE_SIZE;
    particles.Store(addr, asuint(index));

    float lifetime = GetLifeTime(addr);
    if (lifetime <= 0.0)
    {
        ResetParticle(addr);
    }
    else
    {
        UpdateParticle(addr);
    }
}

#endif //! LEGEND_ASSETS_SHADERS_PARTICLE_GPU_PARTICLE_CS_HLSL_
