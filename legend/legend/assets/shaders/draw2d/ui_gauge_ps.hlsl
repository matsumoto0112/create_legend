#ifndef LEGEND_ASSETS_SHADERS_DRAW2D_UI_GAUGE_PS_HLSL
#define LEGEND_ASSETS_SHADERS_DRAW2D_UI_GAUGE_PS_HLSL

/**
 * @file ui_gauge_ps.hlsl
 * @brief
 */

#include "ui_gauge.hlsli"

float4 main(const PSInput i) : SV_TARGET{
    return PSMain(i);
}

#endif //! LEGEND_ASSETS_SHADERS_DRAW2D_UI_GAUGE_PS_HLSL
