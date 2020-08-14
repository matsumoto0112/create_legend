#ifndef LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_
#define LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_

/**
 * @file pixel_shader.h
 * @brief ピクセルシェーダーリソース管理クラス定義
 */

#include "src/directx/shader/pixel_shader.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class PixelShader
 * @brief ピクセルシェーダーリソース管理クラス
 */
class PixelShader
    : public ResourceManager<std::shared_ptr<directx::shader::PixelShader>> {
 public:
  /**
   * @brief コンストラクタ
   */
  PixelShader();
  /**
   * @brief デストラクタ
   */
  virtual ~PixelShader();
  /**
   * @brief リソースを読み込む
   * @param name リソース名
   * @return 読み込みに成功したらtrueを返す
   */
  bool Load(const std::wstring& name) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_
