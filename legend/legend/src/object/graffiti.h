#ifndef LEGEND_OBJECT_GRAFFITI_H_
#define LEGEND_OBJECT_GRAFFITI_H_

/**
 * @file graffiti.h
 */

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/object/fragment.h"
#include "src/util/color_4.h"
#include "src/util/timer.h"
#include "src/util/transform.h"

namespace legend {
namespace object {

/**
 * @brief �������̏������p�����[�^
 */
struct GraffitiInitializeParameter {
  // math::Vector3 position;
  // math::Vector3 scale;
  util::Transform transform;
  math::Vector3 bounding_box_length;
  float remaining_graffiti;
};

/**
 * @brief �������I�u�W�F�N�g�N���X
 */
class Graffiti : public actor::Actor {
  using Parent = actor::Actor;

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
  bool Init(actor::IActorMediator* mediator,
            const GraffitiInitializeParameter& param,
            directx::device::CommandList& command_list);
  /**
   * @brief �X�V����
   */
  bool Update();
  /**
   * @brief �`�揈��
   * @param command_list �R�}���h���X�g
   */
  void Draw(directx::device::CommandList& command_list);
  /**
   * @brief ����������������
   * @param ��������
   */
  void DecreaseGraffiti(const float& percentage);
  /**
   * @brief �������̎c�ʂ̎擾
   */
  float GetRemainingGraffiti() const;
  /**
   * @brief �����Ă��邩���擾
   */
  bool GetIsErase() const;
  /**
   * @brief �����J�X��������
   */
   //void InstanceFragment();

  void OnHit(bullet::Collider* other);

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
  //! �e�N�X�`�����\�[�X
  ComPtr<ID3D12Resource> mask_texture_;
  //! �e�N�X�`���̏��������ɕK�v�ȃR�s�[���\�[�X
  ComPtr<ID3D12Resource> mask_texture_copy_;
  //! �e�N�X�`���n���h��
  directx::descriptor_heap::DescriptorHandle handle_;
  //! �s�N�Z���f�[�^
  std::array<u8, MASK_WIDTH * MASK_HEIGHT * PIXEL_SIZE> pixels_;

  //! �������c��
  float remaining_graffiti_;
  //! ���������ǂ���
  bool is_erase_;
  std::shared_ptr<bullet::BoundingBox> box_;

  util::CountDownTimer delete_time_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_GRAFFITI_H_
