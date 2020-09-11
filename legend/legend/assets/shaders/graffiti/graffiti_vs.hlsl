#ifndef LEGEND_ASSETS_SHADERS_GRAFFITI_GRAFFITI_VS_HLSL_
#define LEGEND_ASSETS_SHADERS_GRAFFITI_GRAFFITI_VS_HLSL_

/**
 * @file graffiti_vs.hlsl
 * @brief 落書き描画用頂点シェーダー
 */

#include "graffiti.hlsli"

VSOutput main(const VSInput v) { return VSMain(v); }

#endif  //! LEGEND_ASSETS_SHADERS_GRAFFITI_GRAFFITI_VS_HLSL_
