#ifndef LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_
#define LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_

/**
 * @file vertex_shader.h
 * @brief 頂点シェーダー定義
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace shader {
class VertexShader {
 public:
  VertexShader();
  ~VertexShader();
  bool Init(DirectX12Device& device, const std::wstring& filepath);
  CD3DX12_SHADER_BYTECODE GetShaderBytecode() const {
    return CD3DX12_SHADER_BYTECODE(vertex_shader_->GetBufferPointer(),
                                   vertex_shader_->GetBufferSize());
  }

 private:
  ComPtr<ID3DBlob> vertex_shader_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_
