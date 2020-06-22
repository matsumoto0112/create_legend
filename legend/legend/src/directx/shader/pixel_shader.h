#ifndef LEGEND_DIRECTX_SHADER_PIXEL_SHADER_H_
#define LEGEND_DIRECTX_SHADER_PIXEL_SHADER_H_

/**
 * @file pixel_shader.h
 * @brief ピクセルシェーダー定義
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace shader {
class PixelShader {
 public:
  PixelShader();
  ~PixelShader();
  bool Init(DirectX12Device& device, const std::wstring& filepath);
  CD3DX12_SHADER_BYTECODE GetShaderBytecode() const {
    return CD3DX12_SHADER_BYTECODE(pixel_shader_->GetBufferPointer(),
                                   pixel_shader_->GetBufferSize());
  }

 private:
  ComPtr<ID3DBlob> pixel_shader_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_PIXEL_SHADER_H_
