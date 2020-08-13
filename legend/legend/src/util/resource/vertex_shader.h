#ifndef LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_
#define LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_

/**
 * @file vertex_shader.h
 * @brief 頂点シェーダーリソース管理クラス定義
 */

#include "src/directx/shader/vertex_shader.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class VertexShader
 * @brief 頂点シェーダーリソース管理クラス
 */
class VertexShader
    : public ResourceManager<std::shared_ptr<directx::shader::VertexShader>> {
 public:
  /**
   * @brief コンストラクタ
   */
  VertexShader();
  /**
   * @brief デストラクタ
   */
  virtual ~VertexShader();
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

#endif  //! LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_
