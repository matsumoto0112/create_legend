#ifndef LEGEND_DIRECTX_SHADER_PIXEL_SHADER_H_
#define LEGEND_DIRECTX_SHADER_PIXEL_SHADER_H_

/**
 * @file pixel_shader.h
 * @brief ピクセルシェーダー定義
 */

#include "src/directx/shader/shader_base.h"

namespace legend {
namespace directx {
namespace shader {

//ピクセルシェーダーはShaderBaseと何も変わらないので再定義するだけ
using PixelShader = ShaderBase;

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_PIXEL_SHADER_H_
