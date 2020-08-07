#ifndef LEGEND_DRAW_SPRITE_RENDERER_H_
#define LEGEND_DRAW_SPRITE_RENDERER_H_

/**
 * @file sprite_renderer.h
 * @brief �X�v���C�g�`��@�\��`�N���X
 */

#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/draw/sprite_2d.h"

namespace legend {
namespace draw {

/**
 * @brief �X�v���C�g�`���
 */
class SpriteRenderer {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  SpriteRenderer();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~SpriteRenderer();
  /**
   * @brief ������
   * @param window_size �E�B���h�E�T�C�Y
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(const math::Vector2& window_size);
  /**
   * @brief �`�惊�X�g�ɃX�v���C�g��ǉ�����
   */
  void AddDrawItems(Sprite2D* sprite);
  /**
   * @brief �`�惊�X�g�ɂ���X�v���C�g��`�悷��
   * @param command_list �R�}���h���X�g
   */
  void DrawItems(directx::device::CommandList& command_list);

 private:
  using WorldContext_CBStruct =
      directx::buffer::constant_buffer_structure::WorldContext;

 private:
  //! ���_�o�b�t�@
  directx::buffer::VertexBuffer vertex_buffer_;
  //! �C���f�b�N�X�o�b�t�@
  directx::buffer::IndexBuffer index_buffer_;
  //! ���[���h���R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<WorldContext_CBStruct> world_cb_;
  //! �`��p�C�v���C��
  directx::shader::GraphicsPipelineState pipeline_state_;
  //! �X�v���C�g���X�g
  std::vector<Sprite2D*> draw_items_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_SPRITE_RENDERER_H_
