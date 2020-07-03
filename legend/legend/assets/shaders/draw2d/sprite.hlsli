#ifndef LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_HLSLI_
#define LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_HLSLI_

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
    output.position = float4(input.position, 1.0f);
    output.uv = input.uv;

    return output;
}

float4 PS_Main(const PS_Input input) {
    //float4 color = float4(input.uv, 0.0f, 0.0f);
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}

#endif //! LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_HLSLI_
