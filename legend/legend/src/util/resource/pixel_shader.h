#ifndef LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_
#define LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_

/**
 * @file pixel_shader.h
 * @brief ピクセルシェーダーリソース管理クラス定義
 */

#include "src/directx/shader/pixel_shader.h"
#include "src/util/resource/resource_id.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class PixelShader
 * @brief ピクセルシェーダーリソース管理クラス
 */
class PixelShader final
    : public ResourceManager<id::PixelShader,
                             std::shared_ptr<directx::shader::PixelShader>> {
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
   * @brief リソースを読み込む
   * @param key リソースを一意に特定するID
   * @param filepath ファイルへのパス
   * @return 読み込みに成功したらtrueを返す
   */
  bool Load(id::PixelShader, const std::filesystem::path& filepath) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_
