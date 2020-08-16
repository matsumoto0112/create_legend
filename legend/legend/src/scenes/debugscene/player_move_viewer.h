#ifndef LEGEND_SCENES_DEBUGSCENE_PLAYER_MOVE_VIEWER_H_
#define LEGEND_SCENES_DEBUGSCENE_PLAYER_MOVE_VIEWER_H_

#include "src/camera/perspective_camera.h"
#include "src/object/desk.h"
#include "src/object/fragment.h"
#include "src/object/graffiti.h"
#include "src/player/player.h"
#include "src/scenes/scene.h"
#include "src/system/mode.h"
#include "src/system/physics_field.h"

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
  //! ���C���J����
  camera::PerspectiveCamera camera_;
  //! �v���C���[
  player::Player player_;
  //! ��
  object::Desk desk_;
  object::Graffiti graffiti_;
  std::vector<object::Fragment> fragments_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_PLAYER_MOVE_VIEWER_H_
