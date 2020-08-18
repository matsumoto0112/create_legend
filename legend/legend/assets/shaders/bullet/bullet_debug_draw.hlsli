#ifndef LEGEND_ASSETS_SHADERS_BULLET_BULLET_DEBUG_DRAW_HLSLI
#define LEGEND_ASSETS_SHADERS_BULLET_BULLET_DEBUG_DRAW_HLSLI

/**
 * @file bullet_debug_draw.hlsli
 * @brief
 */

#include "../defines/global.hlsli"

struct VSInput {
    float4 pos : POSITION;
    float4 color : COLOR0;
};

struct VSOutput {
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};


typedef VSOutput PSInput;

#endif //! LEGEND_ASSETS_SHADERS_BULLET_BULLET_DEBUG_DRAW_HLSLI
