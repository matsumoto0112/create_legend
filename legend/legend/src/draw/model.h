#ifndef LEGEND_DRAW_MODEL_H_
#define LEGEND_DRAW_MODEL_H_

/**
 * @file model.h
 * @brief ���f���Ǘ��N���X
 */

#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/device/command_list.h"

namespace legend {
namespace draw {

/**
 * @brief ���f���Ǘ�
 */
class Model {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Model();
  /**
   * @brief �f�X�g���N�^
   */
  ~Model();
  /**
   * @brief ������
   * @param path ���f���ւ̃p�X
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(const std::filesystem::path& path,
            directx::device::CommandList& command_list);
  /**
   * @brief �`�悷��
   */
  void Draw(directx::device::CommandList& command_list);

 private:
  //! ���f����
  std::wstring model_name_;
  //! ���_�o�b�t�@
  directx::buffer::VertexBuffer vertex_buffer_;
  //! �C���f�b�N�X�o�b�t�@
  directx::buffer::IndexBuffer index_buffer_;
  //! �A���x�h�e�N�X�`��
  directx::buffer::Texture2D albedo_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_MODEL_H_
