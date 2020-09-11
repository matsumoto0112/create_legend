#ifndef LEGEND_ASSETS_SHADERS_MODELVIEW_MODEL_VIEW_PS_HLSL_
#define LEGEND_ASSETS_SHADERS_MODELVIEW_MODEL_VIEW_PS_HLSL_

/**
 * @file model_view_ps.hlsl
 * @brief 通常モデル描画用ピクセルシェーダー
 */

#include "model_view.hlsli"

float4 main(const PSInput i) : SV_TARGET { return PSMain(i); }

#endif  //! LEGEND_ASSETS_SHADERS_MODELVIEW_MODEL_VIEW_PS_HLSL_