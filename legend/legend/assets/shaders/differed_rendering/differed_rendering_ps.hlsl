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

struct LightState {
    float3 position;
    float3 direction;
    float4 color;
    float4x4 view;
    float4x4 proj;
};
ConstantBuffer<LightState> g_light_cb: register(b7);

float4 main(const PSInput i) : SV_TARGET{
    float4 world_position = g_world_position.Sample(g_sampler_warp, i.uv);
    float4 normal_depth = g_world_normal.Sample(g_sampler_warp, i.uv);
    float3 world_normal = normal_depth.xyz * 2.0f - 1.0f;
    float4 diffuse = g_diffuse.Sample(g_sampler_warp, i.uv);

    float4 color = float4(0, 0, 0, 0);
    float3 to_light = world_position.xyz - g_light_cb.position;
    float3 normalized_to_light = normalize(to_light);
    float dotNL = saturate(-dot(normalized_to_light, world_normal));

    float4 light_space_position = world_position;
    light_space_position = mul(light_space_position, g_light_cb.view);
    light_space_position = mul(light_space_position, g_light_cb.proj);

    light_space_position.xyz /= light_space_position.w;

    float light_space_depth = light_space_position.z - 0.00005f;
    float2 shadow_texcoord = 0.5f * light_space_position.xy + 0.5f;
    shadow_texcoord.y = 1.0 - shadow_texcoord.y;

    float shadow_depth = g_shadow_map.Sample(g_sampler_clamp, shadow_texcoord.xy).r;
    float shadow_test = shadow_depth >= light_space_depth ? 1.0 : 0.0;

    //color += diffuse * saturate(g_light_cb.color * dotNL * shadow_test);
    color += shadow_depth;

    color.a = 1.0f;
    return color;
}

#endif //! LEGEND_ASSETS_SHADERS_DIFFERED_RENDERING_DIFFERED_RENDERING_PS_HLSL
