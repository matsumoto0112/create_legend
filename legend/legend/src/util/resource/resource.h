#ifndef LEGEND_UTIL_RESOURCE_RESOURCE_H_
#define LEGEND_UTIL_RESOURCE_RESOURCE_H_

/**
 * @file resource.h
 * @brief リソース管理クラス定義
 */

#include "src/util/resource/model.h"
#include "src/util/resource/pipeline.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/texture.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class Resource
 * @brief リソース管理クラス
 */
class Resource {
 public:
  /**
   * @brief コンストラクタ
   */
  Resource();
  /**
   * @brief デストラクタ
   */
  ~Resource();
  /**
   * @brief 初期化
   * @return 初期化に成功したらtrueを返す
   */
  bool Init();
  /**
   * @brief ピクセルシェーダーを取得する
   */
  resource::PixelShader& GetPixelShader() { return pixel_shader_; }
  /**
   * @brief 頂点シェーダーを取得する
   */
  resource::VertexShader& GetVertexShader() { return vertex_shader_; }
  /**
   * @brief パイプラインを取得する
   */
  resource::Pipeline& GetPipeline() { return pipeline_; }
  /**
   * @brief モデルを取得する
   */
  resource::Model& GetModel() { return model_; }

  resource::Texture& GetTexture() { return texture_; }

 private:
  //! ピクセルシェーダー管理
  resource::PixelShader pixel_shader_;
  //! 頂点シェーダー管理
  resource::VertexShader vertex_shader_;
  //! パイプライン管理
  resource::Pipeline pipeline_;
  //! モデル管理
  resource::Model model_;
  resource::Texture texture_;
};

}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_H_
