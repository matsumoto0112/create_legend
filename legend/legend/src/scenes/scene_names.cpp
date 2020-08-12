#include "src/scenes/scene_names.h"

/**
 * @def CREATE_PAIR
 * @brief �V�[���̎�ނƃV�[���������ƂɃy�A���\�z����
 */
#define CREATE_PAIR(type) \
  { type, #type }

namespace legend {
namespace scenes {
namespace scene_names {

//�V�[�������擾����
std::string Get(SceneType type) {
  static std::unordered_map<SceneType, std::string> NAMES{
      CREATE_PAIR(SceneType::TITLE),
      CREATE_PAIR(SceneType::GAMEOVER),
      CREATE_PAIR(SceneType::MODEL_VIEW),
      CREATE_PAIR(SceneType::SOUND_TEST),
      CREATE_PAIR(SceneType::PHYSICS_TEST),
      CREATE_PAIR(SceneType::SPRITE_TEST),
      CREATE_PAIR(SceneType::MULTI_RENDER_TARGET_TEST),
      CREATE_PAIR(SceneType::PLAYER_MOVE_VIEWER),
      CREATE_PAIR(SceneType::ENEMY_MOVE_VIEWER),
      CREATE_PAIR(SceneType::MAIN_SCENE_1),
      CREATE_PAIR(SceneType::STAGE_GENERATE_TEST),
      CREATE_PAIR(SceneType::GRAFFITI_TEST),
      CREATE_PAIR(SceneType::GPU_PARTICLE_TEST),
  };

  MY_ASSERTION(NAMES.find(type) != NAMES.end(),
               L"���o�^��SceneType���I������܂����B");
  return NAMES.at(type);
}
}  // namespace scene_names
}  // namespace scenes
}  // namespace legend
