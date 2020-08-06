#ifndef LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_VS_HLSL_
#define LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_VS_HLSL_

/**
* @file default_vs.hlsl
*/

#include "default.hlsli"

PSInput main(const VSInput v) {
    return VSMain(v);   
}

#endif //! LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_VS_HLSL_
