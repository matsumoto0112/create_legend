#ifndef LEGEND_ASSETS_SHADERS_BULLET_BULLET_DEBUG_DRAW_PS_HLSL
#define LEGEND_ASSETS_SHADERS_BULLET_BULLET_DEBUG_DRAW_PS_HLSL

/**
 * @file bullet_debug_draw_ps.hlsl
 * @brief bulletデバッグ描画用ピクセルシェーダー
 */

#include "bullet_debug_draw.hlsli"

float4 main(const PSInput i) : SV_TARGET{
    return float4(i.color.rgb, 1.0);
}

#endif //! LEGEND_ASSETS_SHADERS_BULLET_BULLET_DEBUG_DRAW_PS_HLSL
