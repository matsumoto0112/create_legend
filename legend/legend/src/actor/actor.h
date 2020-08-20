#ifndef LEGEND_ACTOR_ACTOR_H_
#define LEGEND_ACTOR_ACTOR_H_

/**
 * @file actor.h
 * @brief �A�N�^�[�N���X��`
 */

#include "src/actor/actor_mediator.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/draw/model.h"
#include "src/util/transform.h"

namespace legend {
namespace actor {

/**
 * @brief �A�N�^�[�N���X
 */
class Actor {
  using TransformConstantBuffer = directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Actor(const std::wstring& name);
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Actor();
  /**
   * @brief ������
   * @param mediator �A�N�^�[����
   */
  virtual bool Init(IActorMediator* mediator);
  /**
   * @brief �X�V����
   * @return �I�u�W�F�N�g���폜����Ƃ���true��Ԃ�
   */
  virtual bool Update() = 0;
  /**
   * @brief �`��
   */
  virtual void Draw();
  virtual void Destroy(){};

 public:
  std::wstring GetName() const { return name_; }
  util::Transform& GetTransformRef() { return transform_; }
  util::Transform GetTransform() const { return transform_; }

 protected:
  //! �A�N�^�[��
  std::wstring name_;
  //! �g�����X�t�H�[��
  util::Transform transform_;
  //! �g�����X�t�H�[���R���X�^���g�o�b�t�@
  TransformConstantBuffer transform_cb_;
  //! �`�惂�f��
  std::shared_ptr<draw::Model> model_;
  //! �����
  IActorMediator* mediator_;
};

}  // namespace actor
}  // namespace legend

#endif  //! LEGEND_ACTOR_ACTOR_H_
