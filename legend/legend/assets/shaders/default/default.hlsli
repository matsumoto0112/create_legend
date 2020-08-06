#ifndef LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_HLSLI_
#define LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_HLSLI_

/**
* @file default.hlsli
*/

struct VSInput{
    float3 position : POSITION;    
};

struct PSInput{
    float4 position : SV_POSITION;    
};

PSInput VSMain(const VSInput v) {
    PSInput result = (PSInput)0;
    result.position = float4(v.position, 1.0);
    
    return result;
}

float4 PSMain(const PSInput i) : SV_TARGET {
    return float4(1.0f, 1.0f, 1.0f, 1.0f);    
}

#endif //! LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_HLSLI_
