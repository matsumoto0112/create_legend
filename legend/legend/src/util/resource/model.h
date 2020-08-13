#ifndef LEGEND_UTIL_RESOURCE_MODEL_H_
#define LEGEND_UTIL_RESOURCE_MODEL_H_

/**
 * @file model.h
 * @brief 読み込んだモデルを管理するクラス定義
 */

#include "src/draw/model.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {
/**
 * @class Model
 * @brief モデルリソース管理クラス
 */
class Model : public ResourceManager<std::shared_ptr<draw::Model>> {
 public:
  /**
   * @brief コンストラクタ
   */
  Model();
  /**
   * @brief デストラクタ
   */
  virtual ~Model();
  /**
   * @brief リソースを読み込む
   * @param name リソース名
   * @return 読み込みに成功したらtrueを返す
   */
  bool Load(const std::wstring& name);
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_MODEL_H_
