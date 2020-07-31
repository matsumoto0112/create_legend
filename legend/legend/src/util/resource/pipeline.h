#ifndef LEGEND_UTIL_RESOURCE_PIPELINE_H_
#define LEGEND_UTIL_RESOURCE_PIPELINE_H_

/**
 * @file pipeline.h
 * @brief パイプラインリソース管理クラス定義
 */

#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/util/resource/resource_id.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class Pipeline
 * @brief パイプラインリソース管理クラス
 */
class Pipeline : public ResourceManager<
                     id::Pipeline,
                     std::shared_ptr<directx::shader::GraphicsPipelineState>> {
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
   * @brief リソースをファイルパスから読み込む（未作成）
   * @return 常にfalseを返す。代わりにRegister関数を使用すること。
   * @details csvなどで外部からパイプラインを構築できるようにしたい
   */
  bool Load(id::Pipeline key, const std::filesystem::path& filepath) override {
    MY_ASSERTION(
        false,
        L"resource::Pipeline::Loadを使用しないでください。\n代わりにresource::"
        L"Pipeline::Registerを使用してください。");
    return false;
  }
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_PIPELINE_H_
