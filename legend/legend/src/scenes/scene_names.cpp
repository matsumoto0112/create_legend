#include "src/scenes/scene_names.h"

/**
 * @def CREATE_PAIR
 * @brief シーンの種類とシーン名をもとにペアを構築する
 */
#define CREATE_PAIR(type) \
  { type, #type }

namespace legend {
namespace scenes {
namespace scene_names {

//シーン名を取得する
std::string Get(SceneType type) {
  static std::unordered_map<SceneType, std::string> NAMES{
      CREATE_PAIR(SceneType::TITLE),
      CREATE_PAIR(SceneType::GAMEOVER),
      CREATE_PAIR(SceneType::MODEL_VIEW),
  };

  return NAMES.at(type);
}
}  // namespace scene_names
}  // namespace scenes
}  // namespace legend
