#ifndef LEGEND_ASSETS_SHADERS_DEFINES_GLOBAL_H_
#define LEGEND_ASSETS_SHADERS_DEFINES_GLOBAL_H_

/**
* @file global.hlsli
* @brief シェーダーのグローバル情報定義
*/

struct Transform
{
    float4x4 world;
};

ConstantBuffer<Transform> g_transform : register(b0);

struct WorldContext
{
    float4x4 view;
    float4x4 projection;
};

ConstantBuffer<WorldContext> g_world_context : register(b1);

Texture2D<float4> g_albedo : register(t0);

SamplerState g_sampler_linear : register(s0);
SamplerState g_sampler_point : register(s1);
 
#endif //! LEGEND_ASSETS_SHADERS_DEFINES_GLOBAL_H_
