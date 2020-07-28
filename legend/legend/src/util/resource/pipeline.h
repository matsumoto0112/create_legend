#ifndef LEGEND_UTIL_RESOURCE_PIPELINE_H_
#define LEGEND_UTIL_RESOURCE_PIPELINE_H_

/**
 * @file pipeline.h
 */

#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @brief パイプラインを一意に特定するID
 */
enum class PipelineID {
  MODEL_VIEW,
};

class Pipeline
    : public ResourceManager<
          PipelineID, std::shared_ptr<directx::shader::GraphicsPipelineState>> {
 public:
  /**
   * @brief コンストラクタ
   */
  Pipeline();
  /**
   * @brief デストラクタ
   */
  ~Pipeline();
  /**
   * @brief 初期化
   * @return 初期化に成功したらtrueを返す
   */
  bool Init() override;
  /**
   * @brief リソースをファイルパスから読み込む（未作成）
   * @return 常にfalseを返す。代わりにRegister関数を使用すること。
   * @details csvなどで外部からパイプラインを構築できるようにしたい
   */
  bool Load(PipelineID key, const std::filesystem::path& filepath) override {
    return false;
  }
  bool Unload(PipelineID key) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_PIPELINE_H_
