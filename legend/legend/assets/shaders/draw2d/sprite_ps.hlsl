#ifndef LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_PS_HLSL_
#define LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_PS_HLSL_

/**
 * @file sprite_ps.hlsl
 * @brief 2Dスプライト描画用ピクセルシェーダー
 */

#include "sprite.hlsli"

float4 main(PS_Input input) : SV_TARGET { return PS_Main(input); }

#endif  //! LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_PS_HLSL_