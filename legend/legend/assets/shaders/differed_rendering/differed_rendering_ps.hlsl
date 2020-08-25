#ifndef LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PS_HLSL
#define LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PS_HLSL

/**
 * @file differed_rendering_ps.hlsl
 * @brief
 */

#include "differed_rendering.hlsli"

texture2D g_world_normal : register(t0);
texture2D g_world_position : register(t1);
texture2D g_diffuse : register(t2);

float4 main(const PSInput i) : SV_TARGET{
    float3 world_position = g_world_position.Sample(g_sampler_warp, i.uv).rgb;
    float3 world_normal = 0;
    float depth = 0;
    {
        float4 normal_depth = g_world_normal.Sample(g_sampler_warp, i.uv);
        world_normal = normal_depth.rgb * 2.0 - 1.0;
        depth = normal_depth.a;
    }

    float4 color = float4(0, 0, 0, 0);
    float4 diffuse = g_diffuse.Sample(g_sampler_warp, i.uv);

    float3 light_position = float3(80,50,20);
    float3 light_color = float3(1.0, 1.0, 1.0);
    float3 light_dir = light_position - world_position;

    float3 L = normalize(light_dir);

    float attenuation = 1.0f;
    color.rgb += attenuation * light_color * diffuse.rgb * max(dot(L, world_normal), 0);

    color.a = 1.0;
    return color;
}

#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PS_HLSL
