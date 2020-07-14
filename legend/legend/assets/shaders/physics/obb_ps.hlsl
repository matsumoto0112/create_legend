#ifndef LEGEND_ASSETS_SHADERS_PHYSICS_OBB_PS_HLSL_
#define LEGEND_ASSETS_SHADERS_PHYSICS_OBB_PS_HLSL_

#include "obb.hlsli"

float4 main(PS_Input input) : SV_TARGET{
    return PS_Main(input);
}

#endif //! LEGEND_ASSETS_SHADERS_PHYSICS_OBB_PS_HLSL_
