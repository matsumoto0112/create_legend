#ifndef LEGEND_UTIL_RESOURCE_RESOURCE_H_
#define LEGEND_UTIL_RESOURCE_RESOURCE_H_

/**
 * @file resource.h
 * @brief リソース管理クラス定義
 */

#include "src/util/loader/archive_loader.h"
#include "src/util/resource/compute_shader.h"
#include "src/util/resource/geometry_shader.h"
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
  virtual ~Resource();
  /**
   * @brief 初期化
   * @return 初期化に成功したらtrueを返す
   */
  bool Init();
  loader::ArchiveLoader& GetArchiveLoader() { return archive_loader_; }

  /**
   * @brief ピクセルシェーダーを取得する
   */
  resource::PixelShader& GetPixelShader() { return pixel_shader_; }
  /**
   * @brief 頂点シェーダーを取得する
   */
  resource::VertexShader& GetVertexShader() { return vertex_shader_; }
  /**
   * @brief ジオメトリシェーダーを取得する
   */
  resource::GeometryShader& GetGeometryShader() { return geometry_shader_; }
  /**
   * @brief コンピュートシェーダーを取得する
   */
  resource::ComputeShader& GetComputeShader() { return compute_shader_; }
  /**
   * @brief パイプラインを取得する
   */
  resource::Pipeline& GetPipeline() { return pipeline_; }
  /**
   * @brief モデルを取得する
   */
  resource::Model& GetModel() { return model_; }
  /**
   * @brief テクスチャを取得する
   */
  resource::Texture& GetTexture() { return texture_; }

 private:
  loader::ArchiveLoader archive_loader_;
  //! ピクセルシェーダー管理
  resource::PixelShader pixel_shader_;
  //! 頂点シェーダー管理
  resource::VertexShader vertex_shader_;
  //! ジオメトリシェーダー管理
  resource::GeometryShader geometry_shader_;
  //! コンピュートシェーダー管理
  resource::ComputeShader compute_shader_;
  //! パイプライン管理
  resource::Pipeline pipeline_;
  //! モデル管理
  resource::Model model_;
  //! テクスチャ管理
  resource::Texture texture_;
};

}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_H_
