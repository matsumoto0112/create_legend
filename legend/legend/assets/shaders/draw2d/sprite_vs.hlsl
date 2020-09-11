#ifndef LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_VS_HLSL_
#define LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_VS_HLSL_

/**
 * @file sprite_vs.hlsl
 * @brief 2Dスプライト描画用頂点シェーダー
 */

#include "sprite.hlsli"

VS_Output main(const VS_Input input) { return VS_Main(input); }

#endif  //! LEGEND_ASSETS_SHADERS_DRAW2D_SPRITE_VS_HLSL_