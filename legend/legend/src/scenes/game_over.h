#ifndef LEGEND_SCENES_GAME_OVER_H_
#define LEGEND_SCENES_GAME_OVER_H_

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {

/**
 *@class GameOver
 *@brief �Q�[���I�[�o�[�V�[��
 */
class GameOver : public Scene {
 public:
  /**
   *@brief �R���X�g���N�^
   */
  GameOver(ISceneChange* scene_change);
  /**
   *@brief ������
   */
  bool Initialize() override;
  /**
   *@brief �X�V
   */
  bool Update() override;
  /**
   *@brief �`��
   */
  void Draw() override;
};
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_GAME_OVER_H_
