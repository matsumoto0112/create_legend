#ifndef LEGEND_ASSETS_SHADERS_GRAFFITI_GRAFFITI_HLSLI_
#define LEGEND_ASSETS_SHADERS_GRAFFITI_GRAFFITI_HLSLI_

/**
 * @file graffiti.hlsli
 * @brief 落書き描画用シェーダー定義
 */

#include "../defines/global.hlsli"

Texture2D<float> g_mask : register(t1);

struct VSInput {
  float3 pos : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD0;
  float4 tangent : TANGENT;
};

struct VSOutput {
  float4 pos : SV_POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD0;
};

typedef VSOutput PSInput;

VSOutput VSMain(const VSInput v) {
  VSOutput o = (VSOutput)0;
  o.pos = mul(float4(v.pos, 1.0), g_transform.world);
  o.pos = mul(o.pos, g_world_context.view);
  o.pos = mul(o.pos, g_world_context.projection);

  o.normal = mul(v.normal, (float3x3)g_transform.world);
  o.normal = v.normal;
  o.uv = v.uv;

  return o;
}

float4 PSMain(const PSInput i) {
  float4 color = g_albedo.Sample(g_sampler_warp, i.uv);
  color = color * g_mask.Sample(g_sampler_warp, i.uv);
  return color;
}

#endif  //! LEGEND_ASSETS_SHADERS_GRAFFITI_GRAFFITI_HLSLI_
