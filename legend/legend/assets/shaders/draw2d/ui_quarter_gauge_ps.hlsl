#ifndef LEGEND_ASSETS_SHADERS_DRAW2D_UI_QUARTER_GAUGE_HLSL
#define LEGEND_ASSETS_SHADERS_DRAW2D_UI_QUARTER_GAUGE_HLSL

/**
 * @file ui_quarter_gauge.hlsl
 * @brief
 */

#include "ui_quarter_gauge.hlsli"

float4 main(const PSInput i) : SV_TARGET{
    return PSMain(i);
}

#endif //! LEGEND_ASSETS_SHADERS_DRAW2D_UI_QUARTER_GAUGE_HLSL
