#ifndef LEGEND_ASSETS_SHADERS_UTIL_UTIL_HLSLI_
#define LEGEND_ASSETS_SHADERS_UTIL_UTIL_HLSLI_

/**
* @file util.hlsli
*/

float Random(float2 uv, uint seed){
    return frac(sin(dot(uv, float2(12.9898, 78.233)) + seed) * 43758.5453123);
}

float RandomRange(float low, float high, float2 uv, uint seed){
    return low + Random(uv, seed) * (high - low);
}

#endif //! LEGEND_ASSETS_SHADERS_UTIL_UTIL_HLSLI_
