#ifndef LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_CS_HLSL
#define LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_CS_HLSL

/**
 * @file player_move_particle_cs.hlsl
 * @brief
 */

#ifndef HLSL
#define HLSL
#endif  //

#include "../defines/global.hlsli"
#include "../util/util.hlsli"
#include "player_move_particle.h"

#define SEED_START (4 * 0)
#define LIFETIME_START (SEED_START + 4 * 1)
#define VELOCITY_START (LIFETIME_START + 4 * 1)
#define POSITION_START (VELOCITY_START + 4 * 3)
#define COLOR_START (POSITION_START + 4 * 3)
#define PARTICLE_SIZE (COLOR_START + 4 * 4)

RWByteAddressBuffer particles : register(u0);

void SetSeed(uint addr, uint value) {
  particles.Store(addr + SEED_START, value);
}

uint GetSeed(uint addr) { return particles.Load(addr + SEED_START); }

void SetLifeTime(uint addr, float value) {
  particles.Store(addr + LIFETIME_START, asuint(value));
}

float GetLifeTime(uint addr) {
  return asfloat(particles.Load(addr + LIFETIME_START));
}

void SetVelocity(uint addr, float3 value) {
  particles.Store3(addr + VELOCITY_START, asuint(value));
}

float3 GetVelocity(uint addr) {
  return asfloat(particles.Load3(addr + VELOCITY_START));
}

void SetPosition(uint addr, float3 value) {
  particles.Store3(addr + POSITION_START, asuint(value));
}

float3 GetPosition(uint addr) {
  return asfloat(particles.Load3(addr + POSITION_START));
}

void SetColor(uint addr, float4 value) {
  particles.Store4(addr + COLOR_START, asuint(value));
}

float4 GetColor(uint addr) {
  return asfloat(particles.Load4(addr + COLOR_START));
}

uint AddrMax() { return DISPATCH_X * THREAD_X * DISPATCH_Y * THREAD_Y; }

void ResetParticle(uint addr, uint index) {
  const float u = (float)index / AddrMax();
  uint seed = GetSeed(addr);

  const int spawn = RandomRange(0, 5, u, seed++);
  const float lifetime = spawn < 3 ? RandomRange(0.25, 0.75, u, seed++) : -1.0;

  SetLifeTime(addr, lifetime);

  const float px = RandomRange(-1.0, 1.0, u, seed++);
  const float py = 0.0;
  const float pz = RandomRange(-1.0, 1.0, u, seed++);
  const float3 pos = g_transform.world._m30_m31_m32 + float3(px, py, pz);
  SetPosition(addr, pos);

  const float vx = RandomRange(-1.0, 1.0, u, seed++);
  const float vy = RandomRange(1.0, 3.0, u, seed++);
  const float vz = RandomRange(-1.0, 1.0, u, seed++);
  const float3 velocity = float3(vx, vy, vz);
  SetVelocity(addr, velocity);

  const uint3 rgb = uint3(192, 192, 192);
  SetColor(addr, float4(rgb / 255.0, 0.2));
  SetSeed(addr, seed);
}

void UpdateParticle(uint addr) {
  float lifetime = GetLifeTime(addr);
  lifetime -= 0.01;
  SetLifeTime(addr, lifetime);
  float3 pos = GetPosition(addr);
  pos += GetVelocity(addr) * g_global.delta_time;
  SetPosition(addr, pos);
}

[numthreads(THREAD_X, THREAD_Y, 1)] void main(const CSInput input) {
  const uint index =
      input.dispatch.z * DISPATCH_X * THREAD_X * DISPATCH_Y * THREAD_Y +
      input.dispatch.y * DISPATCH_X * THREAD_X + input.dispatch.x;

  const uint addr = index * PARTICLE_SIZE;

  float lifetime = GetLifeTime(addr);

  //パーティクルのリセットフラグが立っている
  if (g_particle_info.reset && g_particle_info.emit_enable) {
    ResetParticle(addr, index);
  }
  //生存時間が過ぎて、新しいパーティクルの生成フラグが立っている
  else if (lifetime <= 0.0 && g_particle_info.emit_enable) {
    ResetParticle(addr, index);
  } else {
    UpdateParticle(addr);
  }
}

#endif  //! LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_CS_HLSL
