#ifndef LEGEND_DIRECTX_SHADER_SHADER_BASE_H_
#define LEGEND_DIRECTX_SHADER_SHADER_BASE_H_

/**
 * @file shader_base.h
 * @brief シェーダー基底クラス定義
 */

#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace shader {

/**
 * @class ShaderBase
 * @brief シェーダー基底クラス
 */
class ShaderBase {
 public:
  /**
   * @brief コンストラクタ
   * @return
   */
  ShaderBase(){};
  /**
   * @brief デストラクタ
   */
  virtual ~ShaderBase() = default;
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param filepath ファイルへのパス
   * @return 初期化に成功したらtrueを返す
   */
  virtual bool Init(device::IDirectXAccessor& accessor,
                    const std::filesystem::path& filepath);
  /**
   * @brief シェーダーコードとして返す
   */
  CD3DX12_SHADER_BYTECODE GetShaderBytecode() const {
    return CD3DX12_SHADER_BYTECODE{shader_code_.data(), shader_code_.size()};
  }

 protected:
  //! シェーダーコード
  std::vector<u8> shader_code_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_SHADER_SHADER_BASE_H_
