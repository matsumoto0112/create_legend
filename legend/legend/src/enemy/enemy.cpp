#include "src/enemy/enemy.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/util/path.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace enemy {
//�R���X�g���N�^
Enemy::Enemy()
    : transform_(),
      velocity_(math::Vector3::kZeroVector) {
  transform_.SetScale(math::Vector3(8, 6, 7.5f));
  obb_ = physics::BoundingBox();
  obb_.SetLength(1, 0.5f, 2);
  is_move_ = false;
  impulse_ = 1;
  deceleration_x_ = deceleration_z_ = 0;
}

Enemy::Enemy(math::Vector3 position, math::Quaternion rotation,
               math::Vector3 scale)
    : transform_(),
      velocity_(math::Vector3::kZeroVector) {
  transform_.SetPosition(position);
  transform_.SetRotation(rotation);
  transform_.SetScale(scale);
  //obb_ = physics::BoundingBox(position, rotation, scale);
  obb_.SetLength(1, 1, 2);
  is_move_ = false;
  impulse_ = 1;
  deceleration_x_ = deceleration_z_ = 0;
}

//�f�X�g���N�^
Enemy::~Enemy() {
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  resource.GetVertexShader().Unload(
      util::resource::id::VertexShader::MODEL_VIEW);
  resource.GetPixelShader().Unload(util::resource::id::PixelShader::MODEL_VIEW);
  resource.GetPipeline().Unload(util::resource::id::Pipeline::MODEL_VIEW);
  resource.GetModel().Unload(util::resource::ModelID::OBJECT_1000CM);
}

//������
bool Enemy::Initilaize(directx::DirectX12Device& device) {
  if (!obb_.Initialize(device)) {
    return false;
  }

  //���̃V�[���Ŏg�p����V�F�[�_�[�����O�ɓǂݍ���ł���
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  if (!resource.GetVertexShader().Load(
          util::resource::id::VertexShader::MODEL_VIEW,
          util::Path::GetInstance()->shader() / "modelview" /
              "model_view_vs.cso")) {
    return false;
  }
  if (!resource.GetPixelShader().Load(
          util::resource::id::PixelShader::MODEL_VIEW,
          util::Path::GetInstance()->shader() / "modelview" /
              "model_view_ps.cso")) {
    return false;
  }

  //���f���f�[�^��ǂݍ���
  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / "eraser_01.glb";
  if (!resource.GetModel().Load(util::resource::ModelID::OBJECT_1000CM,
                                model_path)) {
    return false;
  }

  auto gps = std::make_shared<directx::shader::GraphicsPipelineState>();
  gps->Init(device);
  gps->SetVertexShader(resource.GetVertexShader().Get(
      util::resource::id::VertexShader::MODEL_VIEW));
  gps->SetPixelShader(resource.GetPixelShader().Get(
      util::resource::id::PixelShader::MODEL_VIEW));
  gps->SetBlendDesc(directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);
  device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
      device, directx::render_target::RenderTargetID::BACK_BUFFER, gps.get());
  device.GetRenderResourceManager().WriteDepthStencilTargetInfoToPipeline(
      device, directx::render_target::DepthStencilTargetID::Depth, gps.get());
  gps->SetRootSignature(device.GetDefaultRootSignature());
  gps->SetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
  gps->CreatePipelineState(device);
  resource.GetPipeline().Register(util::resource::id::Pipeline::MODEL_VIEW,
                                  gps);

  //�g�����X�t�H�[���o�b�t�@���쐬����
  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                        LocalHeapID::MODEL_VIEW_SCENE),
          L"Transform ConstantBuffer")) {
    return false;
  }

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  return true;
}

//�X�V
bool Enemy::Update() {
  obb_.Update();

  //update_time_ =
  //    game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  //if (is_input_ &&
  //    math::Vector3(change_amount_velocity_ - input_velocity_).Magnitude() >=
  //        1.0f) {
  //  is_move_ = true;
  //}

  //transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  //transform_cb_.UpdateStaging();

  return true;
}

//�`��
void Enemy::Draw(directx::DirectX12Device& device) {
  obb_.Draw(device);

  device.GetRenderResourceManager().SetDepthStencilTargetID(
      directx::render_target::DepthStencilTargetID::Depth);
  device.GetRenderResourceManager().SetRenderTargetsToCommandList(device);
  device.GetRenderResourceManager().ClearCurrentDepthStencilTarget(device);

  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(util::resource::id::Pipeline::MODEL_VIEW)
      ->SetGraphicsCommandList(device);
  transform_cb_.SetToHeap(device);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetModel()
      .Get(util::resource::ModelID::OBJECT_1000CM)
      ->Draw();
}

//�ړ�
void Enemy::Move() {
  if (!is_move_) return;

  //math::Vector3 velocity = math::Vector3::kZeroVector;
  //while (stick_velocities_.size() > 0) {
  //  if (velocity.Magnitude() < stick_velocities_.front().Magnitude()) {
  //    velocity = stick_velocities_.front();
  //    stick_velocities_.erase(stick_velocities_.begin());
  //    continue;
  //  }
  //  stick_velocities_.erase(stick_velocities_.begin());
  //}
  ////�ړ����x���[����������return
  //if (velocity == math::Vector3::kZeroVector) {
  //  ResetParameter();
  //  return;
  //}

  ////�ړ����������߂�
  //float length =
  //    math::util::Sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

  ////���ۂɓ�������
  //float x = velocity.x / length;
  //float z = velocity.z / length;

  ////�����v�Z
  //deceleration_x_ = x / (length * length);
  //deceleration_z_ = z / (length * length);

  ////�ړ�����
  //math::Vector3 v = math::Vector3(x, 0, z);
  //math::Vector3 position = GetPosition() + v * impulse_ * power_ * update_time_;
  //SetPosition(position);

  //Deceleration(2);
}

void Enemy::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  obb_.SetPosition(position);
}

//���x�̐ݒ�
void Enemy::SetVelocity(math::Vector3 velocity) { velocity_ = velocity; }

void Enemy::SetRotation() {
  //input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //math::Quaternion rotation = transform_.GetRotation();
  //rotation.y += input.GetGamepad()->GetStickRight().x;
  //transform_.SetRotation(rotation);
  //obb_.SetRotation(rotation);
}

void Enemy::SetVelocity() {
  if (is_move_) return;

  //input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  ////�Q�[���p�b�h��������ڑ�����Ă����

  //if (input.GetGamepad()->GetCount() != 1) return;
  //input_velocity_.x = -input.GetGamepad()->GetStickLeft().x;
  //input_velocity_.z = -input.GetGamepad()->GetStickLeft().y;

  //velocity_update_time_ += update_time_;
  //if (velocity_update_time_ < change_time_) return;

  ////���X�e�B�b�N�̌X���ɍ��킹�Ēl������
  //velocity_.x = input_velocity_.x;
  //velocity_.z = input_velocity_.z;
  //if (velocity_.Magnitude() >= 0.1f) is_input_ = true;

  //change_amount_velocity_ = velocity_;
  //stick_velocities_.push_back(velocity_);
  //while (stick_velocities_.size() > max_stick_velocity_num_) {
  //  stick_velocities_.erase(stick_velocities_.begin());
  //}
  //velocity_update_time_ = 0;
}

//�p���[�̐ݒ�
void Enemy::SetImpulse() {
  //input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  ////�Q�[���p�b�h��������ڑ�����Ă����
  //if (input.GetGamepad()->GetCount() != 1) return;

  ////�p���[�������I������return
  //if (is_set_power_ || !is_input_) return;

  ////���X�e�B�b�N�̌X����0.1�ȉ����A�������͂��Ă�����p���[�����������ɂ���
  //if (input.GetGamepad()->GetStickLeft().Magnitude() <= 0.1f && is_input_) {
  //  is_set_power_ = true;
  //  return;
  //}

  ////�p���[��������
  //if (up_power_) {
  //  impulse_ += 0.1f * update_time_ * 2;
  //  if (impulse_ >= max_power_) {
  //    impulse_ = max_power_;
  //    up_power_ = false;
  //  }
  //} else {
  //  impulse_ -= 0.1f * update_time_ * 2;
  //  if (impulse_ <= min_power_) {
  //    impulse_ = min_power_;
  //    up_power_ = true;
  //  }
  //}
}

//�ړ��ɕK�v�ȃp�����[�^��������
void Enemy::ResetParameter() {
  if (velocity_.Magnitude() != 0.0f) return;

  impulse_ = 0;
  deceleration_x_ = deceleration_z_ = 0;
  is_move_ = false;
}

//����
void Enemy::Deceleration(float deceleration_rate) {
  //float x = deceleration_x_ * deceleration_rate * update_time_;
  //float z = deceleration_z_ * deceleration_rate * update_time_;

  //if ((x <= velocity_.x && velocity_.x <= 0) ||
  //    (0 <= velocity_.x && velocity_.x <= x)) {
  //  velocity_.x = 0;
  //} else {
  //  velocity_.x -= x;
  //}
  //if ((z <= velocity_.z && velocity_.z <= 0) ||
  //    (0 <= velocity_.z && velocity_.z <= z)) {
  //  velocity_.z = 0;
  //} else {
  //  velocity_.z -= z;
  //}

  //change_amount_velocity_ = velocity_;
  //stick_velocities_.push_back(velocity_);
  //while (stick_velocities_.size() > max_stick_velocity_num_) {
  //  stick_velocities_.erase(stick_velocities_.begin());
  //}
}

//���W�̎擾
math::Vector3 Enemy::GetPosition() const { return transform_.GetPosition(); }

//�ړ��ʂ̎擾
math::Vector3 Enemy::GetVelocity() const { return velocity_; }

math::Quaternion Enemy::GetRotation() const {
  return transform_.GetRotation();
}

float Enemy::GetImpulse() const { return impulse_; }

physics::BoundingBox& Enemy::GetOBB() {
  physics::BoundingBox& obb = obb_;
  return obb;
}
}  // namespace enemy
}  // namespace legend