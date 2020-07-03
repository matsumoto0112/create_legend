#ifndef LEGEND_SCENES_SCENE_NAMES_H_
#define LEGEND_SCENES_SCENE_NAMES_H_

/**
 * @file scene_names.h
 * @brief シーン名定義
 */

#include "src/scenes/scene_manager.h"

namespace legend {
namespace scenes {
namespace scene_names {
/**
 * @brief シーン種類からシーン名を取得する
 * @param type シーンの種類
 * @return シーンの名前
 * @details 未登録のシーンの時はエラーとなる
 */
std::string Get(SceneType type);
}  // namespace scene_names
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_SCENE_NAMES_H_
