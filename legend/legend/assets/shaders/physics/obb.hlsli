#ifndef LEGEND_ASSETS_SHADERS_PHYSICS_OBB_HLSLI_
#define LEGEND_ASSETS_SHADERS_PHYSICS_OBB_HLSLI_

#include "../defines/global.hlsli"

struct VS_Input {
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_Output {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

typedef VS_Output PS_Input;

VS_Output VS_Main(const VS_Input input) {
    VS_Output output = (VS_Output)0;
    output.position = mul(float4(input.position, 1.0f), g_transform.world);
    output.position = mul(output.position, g_world_context.view);
    output.position = mul(output.position, g_world_context.projection);

    output.normal = mul(input.normal, (float3x3)g_transform.world);
    output.normal = input.normal;

    return output;
}

float4 PS_Main(const PS_Input input) {
    float4 color = float4(0.0f, 1.0f, 0.0f, 1.0f);
    return color;
}

#endif //! LEGEND_ASSETS_SHADERS_PHYSICS_OBB_HLSLI_
