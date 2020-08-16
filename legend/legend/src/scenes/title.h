#ifndef LEGEND_SCENES_TITLE_H_
#define LEGEND_SCENES_TITLE_H_

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {

/**
 * @class Title
 * @brief �^�C�g���V�[��
 */
class Title : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Title(ISceneChange* scene_change);
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

  void Finalize() override;

 private:
  i32 bgm_key_;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_TITLE_H_
