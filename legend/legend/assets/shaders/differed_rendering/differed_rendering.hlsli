#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSLI
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSLI

/**
 * @file differed_rendering.hlsli
 * @brief Differed-Rendering用シェーダー定義
 */

#include "../defines/global.hlsli"

struct VSInput {
  float3 position : POSITION;
  float2 uv : TEXCOORD0;
};

struct VSOutput {
  float4 position : SV_POSITION;
  float2 uv : TEXCOORD0;
  float4 light_pos : POSITION;
};

typedef VSOutput PSInput;

struct Camera {
  float3 camera_position;
};
ConstantBuffer<Camera> g_camera : register(b6);

#endif  //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_HLSLI
