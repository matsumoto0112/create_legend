#ifndef LEGEND_SCENES_DECORATOR_PAUSABLE_H_
#define LEGEND_SCENES_DECORATOR_PAUSABLE_H_

/**
 * @file pausable.h
 * @brief �|�[�Y���\�ɂ���f�R���[�g�V�[����`
 */

#include "src/draw/sprite_2d.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace decorator {

/**
 * @class Pausable
 * @brief �|�[�Y�\�ɂ���f�R���[�g�V�[��
 */
class Pausable : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   * @param scene_change �V�[���ύX�C���^�[�t�F�[�X
   * @param inner �����ŊǗ�����V�[��
   */
  Pausable(ISceneChange* scene_change, std::unique_ptr<Scene> inner);
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Pausable();
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
  //! �|�[�Y���Ă��邩
  bool is_pause_;
  //! �|�[�Y���ɕ\������摜
  draw::Sprite2D pause_image_;
};

}  // namespace decorator
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DECORATOR_PAUSABLE_H_
