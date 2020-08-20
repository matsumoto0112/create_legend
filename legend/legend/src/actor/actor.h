#ifndef LEGEND_ACTOR_ACTOR_H_
#define LEGEND_ACTOR_ACTOR_H_

/**
 * @file actor.h
 * @brief �A�N�^�[�N���X��`
 */

#include "src/actor/actor_mediator.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"
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
  ~Actor();
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

 protected:
  /**
   * @brief �o�b�t�@�̏�����
   * @return �������ɐ���������true��Ԃ�
   */
  bool InitBuffer();

 public:
  util::Transform& GetTransformRef() { return transform_; }
  util::Transform GetTransform() const { return transform_; }
  /**
   * @brief �g�����X�t�H�[���R���X�^���g�o�b�t�@�̎Q�Ƃ��擾����
   */
  TransformConstantBuffer& GetTransformConstantBufferRef() {
    return transform_cb_;
  }

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

//�R���X�g���N�^k
inline Actor::Actor(const std::wstring& name) : name_(name) {}

//�f�X�g���N�^
inline Actor::~Actor() {}

inline bool Actor::Init(IActorMediator* mediator) {
  this->mediator_ = mediator;
  return true;
}

//�`��
inline void Actor::Draw() {
  MY_ASSERTION(model_.get(), L"���f�������݂��܂���B");

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::MODEL_VIEW)
      ->SetCommandList(command_list);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  model_->Draw(command_list);
}

inline bool Actor::InitBuffer() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  //�g�����X�t�H�[���o�b�t�@���쐬����
  if (!transform_cb_.Init(
          device,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY),
          name_ + L"_TransformConstantBuffer")) {
    return false;
  }

  return true;
}

}  // namespace actor
}  // namespace legend
#endif  //! LEGEND_ACTOR_ACTOR_H_
