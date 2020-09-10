#ifndef LEGEND_ACTOR_DIFFERED_RENDERING_RENDER_COMMAND_LIST_H
#define LEGEND_ACTOR_DIFFERED_RENDERING_RENDER_COMMAND_LIST_H

/**
 * @file differed_rendering_render_command_list.h
 * @brief
 * Differed-Rendering�p�R�}���h���X�g�N���X��`
 */

#include "src/actor/actor.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/object/back_ground.h"

namespace legend {
namespace actor {
/**
 * @brief Differed-Rendering�p�R�}���h���X�g
 */
class DifferedRenderingRenderCommandList {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  DifferedRenderingRenderCommandList();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~DifferedRenderingRenderCommandList();
  /**
   * @brief �`�惊�X�g�ɒǉ�
   */
  void Push(Actor* actor);
  /**
   * @brief �`�惊�X�g�̃N���A
   */
  void Clear();
  /**
   * @brief �`��p�X�����s����
   */
  void RenderPass();

 private:
  //! �`�惊�X�g
  std::vector<Actor*> actor_list_;
  //! �w�i�I�u�W�F�N�g
  object::BackGround back_ground_;
  directx::buffer::VertexBuffer vertex_buffer_;  //! Diff-Render�p���_�o�b�t�@
  directx::buffer::IndexBuffer
      index_buffer_;  //! Diff-Render�p�C���f�b�N�X�o�b�t�@
};
}  // namespace actor
}  // namespace legend
#endif  //! LEGEND_ACTOR_DIFFERED_RENDERING_RENDER_COMMAND_LIST_H
