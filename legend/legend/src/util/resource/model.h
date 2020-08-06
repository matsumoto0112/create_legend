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
 * @enum ModelID
 * @brief モデルを一意に特定するID
 */
enum class ModelID {
  CHECK_XYZ,
  KARI,
  OBJECT_1000CM,
  ERASER,
  DESK,
};

/**
 * @class Model
 * @brief モデルリソース管理クラス
 */
class Model : public ResourceManager<ModelID, std::shared_ptr<draw::Model>> {
 public:
  /**
   * @brief コンストラクタ
   */
  Model();
  /**
   * @brief デストラクタ
   */
  ~Model();
  /**
   * @brief リソースを読み込む
   * @param key モデルを一意に特定するID
   * @param filepath ファイルへのパス
   * @return 読み込みに成功したらtrueを返す
   */
  bool Load(ModelID key, const std::filesystem::path& filepath,
            directx::device::CommandList& command_list);
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_MODEL_H_
