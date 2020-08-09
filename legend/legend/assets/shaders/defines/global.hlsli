#ifndef LEGEND_ASSETS_SHADERS_DEFINES_GLOBAL_H_
#define LEGEND_ASSETS_SHADERS_DEFINES_GLOBAL_H_

/**
* @file global.hlsli
* @brief シェーダーのグローバル情報定義
*/

/**
* @brief トランスフォーム構造体
*/
struct Transform {
    float4x4 world;
};

ConstantBuffer<Transform> g_transform : register(b0);

/**
* @brief ビュー・プロジェクション行列構造体
*/
struct WorldContext {
    float4x4 view;
    float4x4 projection;
};

ConstantBuffer<WorldContext> g_world_context : register(b1);

/**
* @brief UV矩形
*/
struct UVRect {
    float left;
    float top;
    float width;
    float height;
};

ConstantBuffer<UVRect> g_uv_rect : register(b2);


Texture2D<float4> g_albedo : register(t0);

SamplerState g_sampler_warp : register(s0);
SamplerState g_sampler_clamp : register(s1);
 
#endif //! LEGEND_ASSETS_SHADERS_DEFINES_GLOBAL_H_
