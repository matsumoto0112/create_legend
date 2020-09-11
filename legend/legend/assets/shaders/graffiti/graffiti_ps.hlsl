#ifndef LEGEND_ASSETS_SHADERS_GRAFFITI_GRAFFITI_PS_HLSL_
#define LEGEND_ASSETS_SHADERS_GRAFFITI_GRAFFITI_PS_HLSL_

/**
 * @file graffiti_ps.hlsl
 * @brief 落書き描画用ピクセルシェーダー
 */

#include "graffiti.hlsli"

float4 main(PSInput i) : SV_TARGET { return PSMain(i); }

#endif  //! LEGEND_ASSETS_SHADERS_GRAFFITI_GRAFFITI_PS_HLSL_
