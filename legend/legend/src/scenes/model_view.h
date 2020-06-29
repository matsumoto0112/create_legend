#ifndef LEGEND_SCENES_MODEL_VIEW_H_
#define LEGEND_SCENES_MODEL_VIEW_H_

#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
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

 private:
  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_MODEL_VIEW_H_
