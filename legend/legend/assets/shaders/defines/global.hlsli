#ifndef LEGEND_ASSETS_SHADERS_DEFINES_GLOBAL_H_
#define LEGEND_ASSETS_SHADERS_DEFINES_GLOBAL_H_

/**
* @file global.hlsli
* @brief �V�F�[�_�[�̃O���[�o������`
*/

/**
* @brief �g�����X�t�H�[���\����
*/

struct GlobalData{
    float time;
    float delta_time;
};
ConstantBuffer<GlobalData> g_global : register(b0);

struct Transform {
    float4x4 world;
};

ConstantBuffer<Transform> g_transform : register(b1);

/**
* @brief �r���[�E�v���W�F�N�V�����s��\����
*/
struct WorldContext {
    float4x4 view;
    float4x4 projection;
};

ConstantBuffer<WorldContext> g_world_context : register(b2);

/**
* @brief UV��`
*/
struct UVRect {
    float left;
    float top;
    float width;
    float height;
};

ConstantBuffer<UVRect> g_uv_rect : register(b3);


Texture2D<float4> g_albedo : register(t0);

SamplerState g_sampler_warp : register(s0);
SamplerState g_sampler_clamp : register(s1);
 
#endif //! LEGEND_ASSETS_SHADERS_DEFINES_GLOBAL_H_
