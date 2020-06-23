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

/**
 * @brief ピクセルシェーダー
 */
class PixelShader {
 public:
  /**
   * @brief コンストラクタ
   */
  PixelShader();
  /**
   * @brief デストラクタ
   */
  ~PixelShader();
  /**
   * @brief 初期化
   * @param device DirectX12デバイス
   * @param filepath ファイルパス
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(DirectX12Device& device, const std::filesystem::path& filepath);

  /**
   * @brief シェーダーコードを返す
   */
  CD3DX12_SHADER_BYTECODE GetShaderBytecode() const {
    return CD3DX12_SHADER_BYTECODE{pixel_shader_data.data(), pixel_shader_data.size()};
  }

 private:
  //! ピクセルシェーダーファイル
  std::vector<u8> pixel_shader_data;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_PIXEL_SHADER_H_
