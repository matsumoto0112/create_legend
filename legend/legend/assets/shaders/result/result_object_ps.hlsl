#ifndef LEGEND_ASSETS_SHADERS_RESULT_RESULT_OBJECT_PS_HLSL
#define LEGEND_ASSETS_SHADERS_RESULT_RESULT_OBJECT_PS_HLSL

/**
 * @file result_object_ps.hlsl
 * @brief ���U���g�V�[���I�u�W�F�N�g�`��p�s�N�Z���V�F�[�_�[
 */

#include "result_object.hlsli"

float4 main(const PSInput i) : SV_TARGET { return PSMain(i); }

#endif  //! LEGEND_ASSETS_SHADERS_RESULT_RESULT_OBJECT_PS_HLSL
