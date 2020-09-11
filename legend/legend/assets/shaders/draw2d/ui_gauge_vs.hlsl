#ifndef LEGEND_ASSETS_SHADERS_DRAW2D_UI_GAUGE_VS_HLSL
#define LEGEND_ASSETS_SHADERS_DRAW2D_UI_GAUGE_VS_HLSL

/**
 * @file ui_gauge_vs.hlsl
 * @brief ゲージUI描画用頂点シェーダー定義
 */

#include "ui_gauge.hlsli"

VSOutput main(const VSInput v) { return VSMain(v); }

#endif  //! LEGEND_ASSETS_SHADERS_DRAW2D_UI_GAUGE_VS_HLSL
