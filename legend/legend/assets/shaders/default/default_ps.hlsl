#ifndef LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_PS_HLSL_
#define LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_PS_HLSL_

/**
* @file default_ps.hlsl
*/

#include "default.hlsli"

float4 main(const PSInput i) : SV_TARGET {
    return PSMain(i);
}

#endif //! LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_PS_HLSL_
