#ifndef LEGEND_ASSETS_SHADERS_RESULT_RESULT_OBJECT_VS_HLSL
#define LEGEND_ASSETS_SHADERS_RESULT_RESULT_OBJECT_VS_HLSL

/**
 * @file result_object_vs.hlsl
 * @brief リザルトシーンオブジェクト描画用頂点シェーダー
 */

#include "result_object.hlsli"

VSOutput main(const VSInput v) { return VSMain(v); }

#endif  //! LEGEND_ASSETS_SHADERS_RESULT_RESULT_OBJECT_VS_HLSL
