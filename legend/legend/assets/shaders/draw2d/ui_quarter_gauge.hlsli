#ifndef LEGEND_ASSETS_SHADERS_DRAW2D_UI_QUARTER_GAUGE_HLSLI
#define LEGEND_ASSETS_SHADERS_DRAW2D_UI_QUARTER_GAUGE_HLSLI

/**
 * @file ui_quarter_gauge.hlsli
 * @brief 1.4ÉQÅ[ÉWï`âÊ
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

    output.uv = input.uv * float2(g_uv_rect.width, g_uv_rect.height) + float2(g_uv_rect.left, g_uv_rect.top);

    return output;
}

float Cross(float2 v, float2 w) {
    return v.x * w.y - v.y * w.x;
}

float4 PSMain(const PSInput input) {
    const float PI = 3.1415926;
    const float half_PI = PI / 2;
    const float theta = PI * g_gauge.value * 0.5;

    float2 uv = (input.uv - 0.5) * 2.0f;
    const float2 v = uv;
    const float2 w = float2(sin(theta), cos(theta));
    const float cr = Cross(w, v);
    if (cr < 0.0) discard;
    float4 color = g_albedo.Sample(g_sampler_warp, input.uv);
    clip(color.a - 0.001);

    return color;
}

#endif //! LEGEND_ASSETS_SHADERS_DRAW2D_UI_QUARTER_GAUGE_HLSLI
