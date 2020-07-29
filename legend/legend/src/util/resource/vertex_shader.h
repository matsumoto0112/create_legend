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
 * @enum VertexShaderID
 * @brief 頂点シェーダーを一意に特定するID
 */
enum class VertexShaderID {
  MODEL_VIEW,
  SPRITE,
  MULTI_RENDER_TARGET,
  MULTI_RENDER_TARGET_POST_PROCESS,
  OBB,
};

/**
 * @class VertexShader
 * @brief 頂点シェーダーリソース管理クラス
 */
class VertexShader final
    : public ResourceManager<VertexShaderID,
                             std::shared_ptr<directx::shader::VertexShader>> {
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
   * @brief リソースを読み込む
   * @param key リソースを一意に特定するID
   * @param filepath ファイルへのパス
   * @return 読み込みに成功したらtrueを返す
   */
  bool Load(VertexShaderID key, const std::filesystem::path& filepath) override;
};
}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_
