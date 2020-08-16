#ifndef LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_HLSLI_
#define LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_HLSLI_

#include "../defines/global.hlsli"

struct VS_Input {
    float3 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct VS_Output {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

typedef VS_Output PS_Input;

VS_Output VS_Main(const VS_Input input) {
    VS_Output output = (VS_Output)0;
    output.position = mul(float4(input.position, 1.0), g_transform.world);
    output.position = mul(output.position, g_world_context.view);
    output.position = mul(output.position, g_world_context.projection);
    output.uv = input.uv * float2(g_uv_rect.width, g_uv_rect.height) + float2(g_uv_rect.left, g_uv_rect.top);

    return output;
}

float4 PS_Main(const PS_Input input) {
    float4 color = g_albedo.Sample(g_sampler_warp, input.uv);
    clip(color.a - 0.0001);
    return color;
}

#endif //! LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_HLSLI_
