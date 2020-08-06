#ifndef LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_HLSLI_
#define LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_HLSLI_

/**
* @file default.hlsli
*/

Texture2D albedo : register(t0);
SamplerState g_warp : register(s0);

struct CB_S{
    float m;    
};
ConstantBuffer<CB_S> g_cb : register(b0);

struct VSInput{
    float3 position : POSITION;    
    float2 uv : TEXCOORD0;
};

struct PSInput{
    float4 position : SV_POSITION;  
    float2 uv : TEXCOORD0;
};

PSInput VSMain(const VSInput v) {
    PSInput result = (PSInput)0;
    result.position = float4(v.position, 1.0);
    result.uv = v.uv;
    
    return result;
}

float4 PSMain(const PSInput i) : SV_TARGET {
    float4 color = albedo.Sample(g_warp,i.uv);
    color *= g_cb.m;
    return color;  
}

#endif //! LEGEND_ASSETS_SHADERS_DEFAULT_DEFAULT_HLSLI_
