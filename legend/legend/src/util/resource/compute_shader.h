#ifndef LEGEND_UTIL_RESOURCE_COMPUTE_SHADER_H
#define LEGEND_UTIL_RESOURCE_COMPUTE_SHADER_H
/**
 * @file compute_shader.h
 * @brief
 * コンピュートシェーダーのリソース管理クラス定義
 */

#include "src/directx/shader/compute_shader.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class GeometryShader
 * @brief コンピュートシェーダーのリソース管理クラス
 */
class ComputeShader
    : public ResourceManager<std::shared_ptr<directx::shader::ComputeShader>> {
 public:
  /**
   * @brief コンストラクタ
   */
  ComputeShader();
  /**
   * @brief デストラクタ
   */
  virtual ~ComputeShader();
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

#endif  //! LEGEND_UTIL_RESOURCE_COMPUTE_SHADER_H
