#ifndef LEGEND_SCENES_SCENE_NAMES_H_
#define LEGEND_SCENES_SCENE_NAMES_H_

/**
 * @file scene_names.h
 * @brief �V�[������`
 */

#include "src/scenes/scene_manager.h"

namespace legend {
namespace scenes {
namespace scene_names {
/**
 * @brief �V�[����ނ���V�[�������擾����
 * @param type �V�[���̎��
 * @return �V�[���̖��O
 * @details ���o�^�̃V�[���̎��̓G���[�ƂȂ�
 */
std::string Get(SceneType type);
}  // namespace scene_names
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_SCENE_NAMES_H_
