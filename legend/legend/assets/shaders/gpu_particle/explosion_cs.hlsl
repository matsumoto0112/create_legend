#ifndef LEGEND_ASSETS_SHADERS_GPU_PARTICLE_EXPLOSION_CS_H
#define LEGEND_ASSETS_SHADERS_GPU_PARTICLE_EXPLOSION_CS_H

/**
 * @file explosion_cs.h
 * @brief 爆発時のパーティクル更新シェーダー
 */

#ifndef HLSL
#define HLSL
#endif  //

#include "../defines/global.hlsli"
#include "../util/util.hlsli"
#include "fire_particle.h"

#define SEED_START (4 * 0)
#define LIFETIME_START (SEED_START + 4 * 1)
#define VELOCITY_START (LIFETIME_START + 4 * 1)
#define POSITION_START (VELOCITY_START + 4 * 3)
#define SCALE_START (POSITION_START + 4 * 3)
#define COLOR_START (SCALE_START + 4 * 3)
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

void SetScale(uint addr, float3 value) {
  particles.Store3(addr + SCALE_START, asuint(value));
}

float3 GetScale(uint addr) {
  return asfloat(particles.Load3(addr + SCALE_START));
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

  const float lifetime = 2.0f;
  SetLifeTime(addr, lifetime);

  const float px = RandomRange(-1.0, 1.0, u, seed++) * 0.0;
  const float py = 0.0;
  const float pz = RandomRange(-1.0, 1.0, u, seed++) * 0.0;
  const float3 pos = g_transform.world._m30_m31_m32 + float3(px, py, pz);
  SetPosition(addr, pos);

  const float vx = RandomRange(-1.0, 1.0, u, seed++);
  const float vy = RandomRange(0.0f, 1.0, u, seed++);
  const float vz = RandomRange(-1.0, 1.0, u, seed++);
  const float3 velocity = float3(vx, vy, vz);
  SetVelocity(addr, velocity);

  const float s = 1.0;
  SetScale(addr, float3(s, s, s));

  const uint3 rgb = uint3(192, 128, 0);
  SetColor(addr, float4(rgb * 1.0 / 255.0, 0.35));
  SetSeed(addr, seed);
}

void UpdateParticle(uint addr) {
  float lifetime = GetLifeTime(addr);
  lifetime -= g_global.delta_time;
  SetLifeTime(addr, lifetime);

  const float3 velocity = GetVelocity(addr);

  float3 pos = GetPosition(addr);
  pos += velocity * g_global.delta_time * 2.0;
  SetPosition(addr, pos);

  float3 scale = GetScale(addr);
  scale += 15.0 * g_global.delta_time * 2.0;
  SetScale(addr, scale);
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
#endif  //! LEGEND_ASSETS_SHADERS_GPU_PARTICLE_EXPLOSION_CS_H
