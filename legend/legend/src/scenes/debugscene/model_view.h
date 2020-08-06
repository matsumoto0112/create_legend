#ifndef LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_
#define LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_

/**
 * @brief ���f���`��e�X�g�V�[��
 * @details ���f������ǂݍ��݁A�A���x�h�e�N�X�`���𒣂�����Ԃŕ`�悷��V�[��
 �p�����[�^�Ƃ��ă��f���̍��W�A��]�A�X�P�[�����O�A�J�����̍��W�A��]�Afov������ł���
 */

#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/root_signature.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {

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
   * @brief �f�X�g���N�^
   */
  ~ModelView();
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
   * @brief �I������
   */
  void Finalize() override;

 private:
  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::shader::RootSignature root_signature_;
  ComPtr<ID3D12PipelineState> pipeline_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_
