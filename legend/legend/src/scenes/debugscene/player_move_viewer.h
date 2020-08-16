#ifndef LEGEND_SCENES_DEBUGSCENE_PLAYER_MOVE_VIEWER_H_
#define LEGEND_SCENES_DEBUGSCENE_PLAYER_MOVE_VIEWER_H_

#include "src/scenes/scene.h"
#include "src/system/turn_system.h"

namespace legend {
namespace scenes {
namespace debugscene {

class PlayerMoveViewer : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PlayerMoveViewer(ISceneChange* scene_change);
  /**
   * @brief �f�X�g���N�^
   */
  ~PlayerMoveViewer();
  /**
   * @brief ������
   */
  bool Initialize() override;
  /**
   * @brief �X�V
   */
  bool Update() override;
  /**
   * @brief �`��
   */
  void Draw() override;
  /**
   * @brief �I��
   */
  void Finalize() override;

 private:
  system::TurnSystem turn_system_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_PLAYER_MOVE_VIEWER_H_
