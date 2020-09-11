#ifndef LEGEND_ASSETS_SHADERS_RESULT_RESULT_OBJECT_HLSLI
#define LEGEND_ASSETS_SHADERS_RESULT_RESULT_OBJECT_HLSLI

/**
 * @file result_object.hlsli
 * @brief リザルトシーンオブジェクト描画用シェーダー定義
 */

#include "../defines/global.hlsli"

struct VSInput {
  float3 pos : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD0;
  float4 tangent : TANGENT;
};

struct VSOutput {
  float4 pos : SV_POSITION;
  float4 world_pos : WORLD_POS;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD0;
};

typedef VSOutput PSInput;

VSOutput VSMain(const VSInput v) {
  VSOutput o = (VSOutput)0;
  o.pos = mul(float4(v.pos, 1.0), g_transform.world);
  o.world_pos = o.pos;
  o.pos = mul(o.pos, g_world_context.view);
  o.pos = mul(o.pos, g_world_context.projection);

  o.normal = mul(v.normal, (float3x3)g_transform.world);
  o.uv = v.uv;

  return o;
}

float4 PSMain(const PSInput i) {
  float3 world_position = i.world_pos.xyz;
  float3 world_normal = i.normal;

  float4 diffuse = g_albedo.Sample(g_sampler_warp, i.uv);
  float4 color = (float4)0;

  //ライトの座標は固定で計算する
  float3 light_position = float3(200, 200, 100);
  float3 light_color = float3(1.0, 1.0, 1.0);
  float3 light_dir = light_position - world_position;

  float3 L = normalize(light_dir);

  float attenuation = 1.5f;
  color.rgb +=
      attenuation * light_color * diffuse.rgb * max(dot(L, world_normal), 0);

  color.a = 1.0;
  return color;
}

#endif  //! LEGEND_ASSETS_SHADERS_RESULT_RESULT_OBJECT_HLSLI
