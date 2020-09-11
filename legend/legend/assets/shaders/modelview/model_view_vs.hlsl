#ifndef LEGEND_ASSETS_SHADERS_MODELVIEW_MODEL_VIEW_VS_HLSL_
#define LEGEND_ASSETS_SHADERS_MODELVIEW_MODEL_VIEW_VS_HLSL_

/**
 * @file model_view_vs.hlsl
 * @brief 通常モデル描画用頂点シェーダー
 */

#include "model_view.hlsli"

VSOutput main(const VSInput v) { return VSMain(v); }

#endif  //! LEGEND_ASSETS_SHADERS_MODELVIEW_MODEL_VIEW_VS_HLSL_
