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
 * @enum PixelShaderID
 * @brief ピクセルシェーダーを一意に特定するID
 */
enum class PixelShaderID {
  MODEL_VIEW,
  SPRITE,
  MULTI_RENDER_TARGET,
  MULTI_RENDER_TARGET_POST_PROCESS,
  OBB,
};

/**
 * @class PixelShader
 * @brief ピクセルシェーダーリソース管理クラス
 */
class PixelShader final
    : public ResourceManager<PixelShaderID,
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
  bool Load(PixelShaderID key, const std::filesystem::path& filepath) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_
