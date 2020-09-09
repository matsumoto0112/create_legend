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
      CREATE_PAIR(SceneType::TITLE),        CREATE_PAIR(SceneType::GAMEOVER),
      CREATE_PAIR(SceneType::MODEL_VIEW),   CREATE_PAIR(SceneType::SOUND_TEST),
      CREATE_PAIR(SceneType::MAIN_SCENE_1), CREATE_PAIR(SceneType::SKILL_TEST),
  };

  MY_ASSERTION(NAMES.find(type) != NAMES.end(),
               L"未登録のSceneTypeが選択されました。");
  return NAMES.at(type);
}
}  // namespace scene_names
}  // namespace scenes
}  // namespace legend
