#ifndef LEGEND_SCENES_SCENE_H_
#define LEGEND_SCENES_SCENE_H_

#include "src/scenes/scene_change.h"
#include "src/scenes/task.h"

namespace legend {
namespace scenes {

/**
 * @class Scene
 * @brief �V�[���̊��N���X
 */
class Scene : public Task {
 protected:
  ISceneChange* scene_change_;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Scene(ISceneChange* scene_change);
  /**
   * @brief ���z�f�X�g���N�^
   */
  virtual ~Scene() {}
  /**
   * @brief ������
   */
  virtual void Initialize() override {}
  /**
   * @brief �I��
   */
  virtual void Finalize() override;
  /**
   * @brief �X�V
   */
  virtual void Update() override {}
  /**
   * @brief �`��
   */
  virtual void Draw() override;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_SCENE_H_
