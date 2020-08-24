#ifndef LEGEND_SCENES_DECORATOR_FADE_IN_OUT_H
#define LEGEND_SCENES_DECORATOR_FADE_IN_OUT_H

/**
 * @file fade_in_out.h
 * @brief
 */

#include "src/draw/sprite_2d.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace decorator {
class FadeInOut : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   * @param scene_change �V�[���ύX�C���^�[�t�F�[�X
   * @param inner �����ŊǗ�����V�[��
   */
  FadeInOut(ISceneChange* scene_change, std::unique_ptr<Scene> inner);
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~FadeInOut();
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
  //! �����Ǘ��N���X
  std::unique_ptr<Scene> inner_;
  //! �|�[�Y���ɕ\������摜
  draw::Sprite2D pause_image_;
};

}  // namespace decorator
}  // namespace scenes
}  // namespace legend
#endif  //! LEGEND_SCENES_DECORATOR_FADE_IN_OUT_H
