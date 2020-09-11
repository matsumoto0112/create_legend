#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_HLSLI
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_HLSLI

/**
 * @file differed_rendering_pre.hlsli
 * @brief Differed-Rendering事前描画用シェーダー定義
 */

#include "../defines/global.hlsli"

struct VSInput {
  float3 position : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD0;
  float4 tangent : TANGENT;
};

struct VSOutput {
  float4 position : SV_POSITION;
  float4 world_pos : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD0;
};

typedef VSOutput PSInput;

struct PSOutput {
  float4 normal : SV_TARGET0;
  float4 position : SV_TARGET1;
  float4 diffuse : SV_TARGET2;
};

#endif  //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PRE_HLSLI
