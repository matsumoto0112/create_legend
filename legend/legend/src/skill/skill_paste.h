#ifndef LEGEND_SKILL_SKILL_PASTE_H_
#define LEGEND_SKILL_SKILL_PASTE_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"

namespace legend {
namespace skill {
class SkillPaste : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  SkillPaste();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~SkillPaste();
  /**
   * @brief ������
   */
  void Init(math::Vector3 position, actor::IActorMediator* mediator);
  /**
   * @brief �X�V
   */
  bool Update();
  /**
   * @brief �`��
   */
  void Draw();
  /**
   * @brief �폜
   */
  void Destroy(actor::IActorMediator* mediator);
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
  //! �R���C�_�[
  std::shared_ptr<bullet::BoundingBox> box_;
  //! �e�N�X�`�����\�[�X
  ComPtr<ID3D12Resource> mask_texture_;
  //! �e�N�X�`���̏��������ɕK�v�ȃR�s�[���\�[�X
  ComPtr<ID3D12Resource> mask_texture_copy_;
  //! �e�N�X�`���n���h��
  directx::descriptor_heap::DescriptorHandle handle_;
  //! �s�N�Z���f�[�^
  std::array<u8, MASK_WIDTH * MASK_HEIGHT * PIXEL_SIZE> pixels_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_PASTE_H_
