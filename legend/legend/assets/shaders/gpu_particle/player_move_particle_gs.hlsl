#ifndef LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_GS_HLSL
#define LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_GS_HLSL

/**
 * @file player_move_particle_gs.hlsl
 * @brief
 */

#ifndef HLSL
#define HLSL
#endif

#include "player_move_particle.h"

#define MAX_VERTEX_COUNT 6
[maxvertexcount(MAX_VERTEX_COUNT)] void main(
    point GSInput input[1], inout TriangleStream<GSOutput> output) {
  //メッシュの原点の情報
  const float4 position = input[0].position;
  const float4 color = input[0].color;

  //板ポリを構成する4頂点の情報
  const float4 vertex_positions[4] = {
      float4(-0.5f, 0.5f, 0.0f, 1.0f),
      float4(0.5f, 0.5f, 0.0f, 1.0f),
      float4(0.5f, -0.5f, 0.0f, 1.0f),
      float4(-0.5f, -0.5f, 0.0f, 1.0f),
  };

  const float2 vertex_uvs[4] = {float2(0.0, 0.0), float2(1.0, 0.0),
                                float2(1.0, 1.0), float2(0.0, 1.0)};

  //板ポリのインデックス配列
  const uint indices[MAX_VERTEX_COUNT] = {
      0, 1, 2, 0, 2, 3,
  };

  const float scale = 1.0;
  float4x4 world = float4x4(scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0,
                            position.x, position.y, position.z, 1);
  float4x4 view = g_world_context.view;
  float4x4 projection = g_world_context.projection;

  GSOutput result = (GSOutput)0;

  uint i;
  [unroll] for (i = 1; i <= MAX_VERTEX_COUNT; i++) {
    const uint index = indices[i - 1];
    const float4 vertex_position = vertex_positions[index];
    const float2 vertex_uv = vertex_uvs[index];

    float4 view_pos = float4(0, 0, 0, 1);
    view_pos = mul(view_pos, world);
    view_pos = mul(view_pos, view);

    float3 scale_rotate = mul((vertex_position).xyz, (float3x3)world);
    view_pos.xyz += float3(scale_rotate.xy, -scale_rotate.z);

    result.position = mul(float4(view_pos.xyz, 1.0), projection);
    result.uv = vertex_uv;
    result.color = color;

    output.Append(result);
    if (i % 3 == 0) output.RestartStrip();
  }
}

#endif  //! LEGEND_ASSETS_SHADERS_GPU_PARTICLE_PLAYER_MOVE_PARTICLE_GS_HLSL