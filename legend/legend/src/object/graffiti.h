#ifndef LEGEND_OBJECT_GRAFFITI_H_
#define LEGEND_OBJECT_GRAFFITI_H_

/**
 * @file graffiti.h
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/util/color_4.h"
#include "src/util/transform.h"

namespace legend {
namespace object {

/**
 * @brief �������̏������p�����[�^
 */
struct GraffitiInitializeParameter {
  math::Vector3 position;
  math::Vector3 scale;
};

/**
 * @brief �������I�u�W�F�N�g�N���X
 */
class Graffiti {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Graffiti();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Graffiti();
  /**
   * @brief ������
   * @param param �������p�����[�^
   * @param command_list �R�}���h���X�g
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(const GraffitiInitializeParameter& param,
            directx::device::CommandList& command_list);
  /**
   * @brief �X�V����
   */
  void Update();
  /**
   * @brief �`�揈��
   * @param command_list �R�}���h���X�g
   */
  void Draw(directx::device::CommandList& command_list);

 private:
  /**
   * @brief �e�N�X�`���̐F������������
   * @param x UV���W
   * @param x UV���W
   * @param color ����������F
   */
  void SetTextureColor(u32 x, u32 y, const util::Color4& color);
  /**
   * @brief �e�N�X�`�����X�V����
   * @param command_list �R�}���h���X�g
   */
  void UpdateTexture(directx::device::CommandList& command_list);

 private:
  //! �s�N�Z���̃o�C�g�T�C�Y
  static constexpr u32 PIXEL_SIZE = 4;
  //! �}�X�N�摜�̕�
  static constexpr u32 MASK_WIDTH = 16;
  //! �}�X�N�摜�̍���
  static constexpr u32 MASK_HEIGHT = 16;

 private:
  //! �g�����X�t�H�[��
  util::Transform transform_;
  //! �g�����X�t�H�[���R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_cb_;

  //! �e�N�X�`�����\�[�X
  ComPtr<ID3D12Resource> mask_texture_;
  //! �e�N�X�`���̏��������ɕK�v�ȃR�s�[���\�[�X
  ComPtr<ID3D12Resource> mask_texture_copy_;
  //! �e�N�X�`���n���h��
  directx::descriptor_heap::DescriptorHandle handle_;
  //! �s�N�Z���f�[�^
  std::array<u8, MASK_WIDTH * MASK_HEIGHT * PIXEL_SIZE> pixels_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_GRAFFITI_H_
