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
    output.position = mul(float4(input.position, 1.0f), g_world_context.projection);
    output.uv = input.uv;

    return output;
}

float4 PS_Main(const PS_Input input) {
    return g_albedo.Sample(g_sampler_linear, input.uv);
}

#endif //! LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_HLSLI_
