#ifndef LEGEND_UTIL_RESOURCE_GEOMETRY_SHADER_H
#define LEGEND_UTIL_RESOURCE_GEOMETRY_SHADER_H

/**
 * @file geometry_shader.h
 * @brief ジオメトリシェーダーのリソース管理定義
 */

#include "src/directx/shader/geometry_shader.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class GeometryShader
 * @brief ジオメトリシェーダーのリソース管理クラス
 */
class GeometryShader
    : public ResourceManager<std::shared_ptr<directx::shader::GeometryShader>> {
 public:
  /**
   * @brief コンストラクタ
   */
  GeometryShader();
  /**
   * @brief デストラクタ
   */
  virtual ~GeometryShader();
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
#endif  //! LEGEND_UTIL_RESOURCE_GEOMETRY_SHADER_H
