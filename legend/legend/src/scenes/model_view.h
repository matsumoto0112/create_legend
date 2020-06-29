#ifndef LEGEND_SCENES_MODEL_VIEW_H_
#define LEGEND_SCENES_MODEL_VIEW_H_

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {

/**
 * @class ModelView
 * @brief ���f���`��e�X�g�V�[��
 */
class ModelView : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  ModelView(ISceneChange* scene_change);
  /**
   * @brief ������
   */
  void Initialize() override;
  /**
   * @brief �X�V
   */
  void Update() override;
  /**
   * @brief �`��
   */
  void Draw() override;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_MODEL_VIEW_H_
