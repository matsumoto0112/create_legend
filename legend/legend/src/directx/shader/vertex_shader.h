#ifndef LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_
#define LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_

/**
 * @file vertex_shader.h
 * @brief 頂点シェーダー定義
 */

#include "src/directx/directx_accessor.h"
#include "src/directx/shader/shader_base.h"

namespace legend {
namespace directx {
namespace shader {
/**
 * @brief 頂点シェーダー
 */
class VertexShader final : public ShaderBase {
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
   * @brief 初期化
   * @param accessor DirectX12デバイスアクセサ
   * @param filepath シェーダーファイルパス
   * @return 初期化に成功したらtrueを返す
   * @details
   * シェーダーリフレクションによって入力レイアウトを解析するので読み取れない種類があったら上の入力エレメントを直接渡すほうを使用する
   */
  bool Init(IDirectXAccessor& accessor,
            const std::filesystem::path& filepath) override;
  /**
   * @brief 入力レイアウトを返す
   */
  D3D12_INPUT_LAYOUT_DESC GetInputLayout() const {
    return D3D12_INPUT_LAYOUT_DESC{elements_.data(),
                                   static_cast<u32>(elements_.size())};
  }

 private:
  //! 入力レイアウト
  std::vector<D3D12_INPUT_ELEMENT_DESC> elements_;
  //! シェーダーリフレクションデータ
  ComPtr<ID3D12ShaderReflection> shader_refrection_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_
