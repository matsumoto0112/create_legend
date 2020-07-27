#ifndef LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_
#define LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_

/**
 * @file vertex_shader.h
 * @brief 頂点シェーダー定義
 */

#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
namespace shader {
/**
 * @brief 頂点シェーダー
 */
class VertexShader {
 public:
  /**
   * @brief コンストラクタ
   */
  VertexShader();
  /**
   * @brief デストラクタ
   */
  ~VertexShader();
  /**
   * @brief 初期化
   * @param device DirectXデバイス
   * @param filepath シェーダーファイルパス
   * @param elements 入力レイアウト
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, const std::filesystem::path& filepath,
            const std::vector<D3D12_INPUT_ELEMENT_DESC>& elements);
  /**
   * @brief シェーダーコードとして返す
   */
  CD3DX12_SHADER_BYTECODE GetShaderBytecode() const {
    return CD3DX12_SHADER_BYTECODE{shader_code_.data(), shader_code_.size()};
  }
  /**
   * @brief 入力レイアウトを返す
   */
  D3D12_INPUT_LAYOUT_DESC GetInputLayout() const {
    return D3D12_INPUT_LAYOUT_DESC{elements_.data(),
                                   static_cast<u32>(elements_.size())};
  }

 private:
  //! シェーダーファイル
  ComPtr<ID3DBlob> vertex_shader_;

  std::vector<u8> shader_code_;
  //! 入力レイアウト
  std::vector<D3D12_INPUT_ELEMENT_DESC> elements_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_
