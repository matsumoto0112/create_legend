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
texture2D g_shadow_map : register(t3);

struct LightCBStruct {
    float4x4 view;
    float4x4 proj;
};
ConstantBuffer<LightCBStruct> g_light_cb: register(b7);

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

    float3 light_position = float3(30, 20, 5);
    float3 light_color = float3(1.0, 1.0, 1.0);
    float3 light_dir = light_position - world_position;

    float3 L = normalize(light_dir);

    float attenuation = 1.5f;
    color.rgb += attenuation * light_color * diffuse.rgb * max(dot(L, world_normal), 0);

    color.a = 1.0;

    //float4 position = g_world_position.Sample(g_sampler_warp, i.uv);
    //float4 diffuse = 

    //float3 world_position = g_world_position.Sample(g_sampler_warp, i.uv).rgb;
    //float3 world_normal = 0;
    //float depth = 0;
    //{
    //    float4 normal_depth = g_world_normal.Sample(g_sampler_warp, i.uv);
    //    world_normal = normal_depth.rgb * 2.0 - 1.0;
    //    depth = normal_depth.a;
    //}

    //float4 color = float4(0, 0, 0, 0);
    //float4 diffuse = g_diffuse.Sample(g_sampler_warp, i.uv);

    //color = diffuse;

    //float4 p = mul(float4(world_position, 1.0), g_light_cb.view);
    //p = mul(p, g_light_cb.proj);
    //p.xyz = p.xyz / p.w;
    //p.x = (1.0f + p.x) / 2.0f;
    //p.y = (1.0f - p.y) / 2.0f;

    //float sm = g_shadow_map.Sample(g_sampler_warp, i.uv).r;
    //float sma = (p.z - 0.05 < sm) ? 1.0 : 0.0;
    ////color = float4(sm,p.z,0.0, 1.0);
    //color = float4(sm,sm,sm, 1.0);
    ////color += diffuse;
    return color;
}

#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PS_HLSL
