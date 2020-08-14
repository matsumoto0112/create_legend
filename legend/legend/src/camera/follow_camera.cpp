#include "src/camera/follow_camera.h"
namespace legend {
namespace camera {

//�R���X�g���N�^
FollowCamera::FollowCamera() {}

//�f�X�g���N�^
FollowCamera::~FollowCamera() {}

//������
bool FollowCamera::Init(const std::wstring& name, actor::Actor* target,
                        const math::Vector3& offset,
                        const math::Quaternion& rotation, float fov,
                        float aspect_ratio, const math::Vector3& up_vector,
                        float near_z, float far_z) {
  this->follow_target_ = target;
  this->offset_ = offset;
  const math::Vector3 position = target->GetTransform().GetPosition() + offset;
  return PerspectiveCamera::Init(name, position, rotation, fov, aspect_ratio,
                                 up_vector, near_z, far_z);
}

//�R���X�^���g�o�b�t�@�̍X�V
void FollowCamera::UpdateConstantBufferStaging() {
  SetPosition(follow_target_->GetTransform().GetPosition() + offset_);
  PerspectiveCamera::UpdateConstantBufferStaging();
}

}  // namespace camera
}  // namespace legend