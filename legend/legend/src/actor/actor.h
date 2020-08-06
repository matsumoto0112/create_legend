#ifndef LEGEND_ACTOR_ACTOR_H_
#define LEGEND_ACTOR_ACTOR_H_

/**
 * @file actor.h
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_id.h"
#include "src/util/transform.h"

namespace legend {
namespace actor {

/**
 * @brief �A�N�^�[�N���X
 * @tparam T
 */
template <class T>
class Actor {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Actor();
  /**
   * @brief �f�X�g���N�^
   */
  ~Actor();
  /**
   * @brief �X�V����
   * @return �I�u�W�F�N�g���폜����Ƃ���true��Ԃ�
   */
  virtual bool Update() = 0;
  /**
   * @brief �`��
   */
  virtual void Draw();
  /**
   * @brief �R���W�������擾����
   */
  T& GetCollisionRef() { return collision_; }

 protected:
  //! �g�����X�t�H�[��
  util::Transform transform_;
  //! �g�����X�t�H�[���R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_cb_;

  //! �`�惂�f��
  std::shared_ptr<draw::Model> model_;
  //! �R���W����
  T collision_;
};

template <class T>
inline Actor<T>::Actor() {}

template <class T>
inline Actor<T>::~Actor() {}

template <class T>
inline void Actor<T>::Draw() {
  MY_ASSERTION(model_.get(), L"���f�������݂��܂���B");

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.SetToHeap(device);

  model_->Draw();
}

}  // namespace actor
}  // namespace legend
#endif  //! LEGEND_ACTOR_ACTOR_H_
