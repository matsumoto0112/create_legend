#ifndef LEGEND_ASSETS_SHADERS_DRAW2D_UI_GAUGE_HLSLI
#define LEGEND_ASSETS_SHADERS_DRAW2D_UI_GAUGE_HLSLI

/**
 * @file ui_gauge.hlsli
 * @brief ゲージUI描画用シェーダー定義
 */

#include "../defines/global.hlsli"

struct GaugeParameter {
  float value;
};
ConstantBuffer<GaugeParameter> g_gauge : register(b5);

struct VSInput {
  float3 position : POSITION;
  float2 uv : TEXCOORD0;
};

struct VSOutput {
  float4 position : SV_POSITION;
  float2 uv : TEXCOORD0;
};

typedef VSOutput PSInput;

VSOutput VSMain(const VSInput input) {
  VSOutput output = (VSOutput)0;

  output.position = mul(float4(input.position, 1.0), g_transform.world);
  output.position = mul(output.position, g_world_context.view);
  output.position = mul(output.position, g_world_context.projection);

  output.uv = input.uv * float2(g_uv_rect.width, g_uv_rect.height) +
              float2(g_uv_rect.left, g_uv_rect.top);

  return output;
}

float4 PSMain(const PSInput input) {
  float4 color = g_albedo.Sample(g_sampler_warp, input.uv);
  if (input.uv.x > g_gauge.value) {
    color.a = 0.0;
  }
  clip(color.a - 0.0001);
  return color;
}

#endif  //! LEGEND_ASSETS_SHADERS_DRAW2D_UI_GAUGE_HLSLI
