#ifndef LEGEND_DRAW_MODEL_H_
#define LEGEND_DRAW_MODEL_H_

/**
 * @file model.h
 * @brief ���f���Ǘ��N���X
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/math/quaternion.h"

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
  bool Init(const std::filesystem::path& path);
  /**
   * @brief �`�悷��
   */
  void Draw();

 private:
  //! ���f����
  std::wstring model_name_;
  //! ���W
  math::Vector3 position_;
  //! ��]
  math::Quaternion rotation_;
  //! �X�P�[�����O
  math::Vector3 scale_;
  //! ���_�o�b�t�@
  directx::buffer::VertexBuffer vertex_buffer_;
  //! �C���f�b�N�X�o�b�t�@
  directx::buffer::IndexBuffer index_buffer_;
  //! �A���x�h�e�N�X�`��
  directx::buffer::Texture2D albedo_;
  //! �g�����X�t�H�[���p�R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_constant_buffer_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_MODEL_H_
