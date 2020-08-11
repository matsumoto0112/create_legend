#include "src/scenes/debugscene/player_move_viewer.h"

#include "src/physics/collision.h"

namespace legend {
namespace scenes {
namespace debugscene {
//�R���X�g���N�^
PlayerMoveViewer::PlayerMoveViewer(ISceneChange* scene_change)
    : Scene(scene_change) {}

//�f�X�g���N�^
PlayerMoveViewer::~PlayerMoveViewer() {}

//������
bool PlayerMoveViewer::Initialize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  //�v���C���[�̏�����
  {
    player::Player::InitializeParameter player_parameter;
    player_parameter.transform =
        util::Transform(math::Vector3::kZeroVector, math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    player_parameter.bouding_box_length = math::Vector3(0.1f, 0.05f, 0.2f);
    player_parameter.min_power = 0;
    player_parameter.max_power = 1;
    if (!player_.Init(player_parameter)) {
      return false;
    }
  }

  //���̏�����
  {
    //�{���̓X�e�[�W�f�[�^����ǂݍ���
    object::Desk::InitializeParameter desk_parameter;
    desk_parameter.transform =
        util::Transform(math::Vector3::kZeroVector, math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    desk_parameter.bounding_box_length = math::Vector3(0.3f, 0.05f, 0.2f);
    desk_parameter.normal = math::Vector3::kUpVector;
    if (!desk_.Init(desk_parameter)) {
      return false;
    }
  }

  //�J�����̏�����
  {
    const math::Vector3 camera_position = math::Vector3(0, 3.5f, -2.5f);
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
    if (!camera_.Init(L"MainCamera", camera_position, camera_rotation,
                      math::util::DEG_2_RAD * 50.0f, aspect_ratio)) {
      return false;
    }
  }

  return true;
}

//�X�V
bool PlayerMoveViewer::Update() {
  if (!player_.Update()) {
    return false;
  }

  if (ImGui::Begin("Camera")) {
    //�J�������W
    math::Vector3 camera_position = camera_.GetPosition();
    ImGui::SliderFloat3("Position", &camera_position.x, -100.0f, 100.0f);
    camera_.SetPosition(camera_position);
    //�J������]�p
    math::Vector3 camera_rotation =
        math::Quaternion::ToEular(camera_.GetRotation()) *
        math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &camera_rotation.x, -180.0f, 180.0f);
    camera_.SetRotation(
        math::Quaternion::FromEular(camera_rotation * math::util::DEG_2_RAD));

    //�J�����̏�����x�N�g����ύX����
    if (ImGui::Button("X_UP")) {
      camera_.SetUpVector(math::Vector3::kRightVector);
    }
    if (ImGui::Button("Y_UP")) {
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("Z_UP")) {
      camera_.SetUpVector(math::Vector3::kForwardVector);
    }
    float fov = camera_.GetFov() * math::util::RAD_2_DEG;
    ImGui::SliderFloat("FOV", &fov, 0.01f, 90.0f);
    camera_.SetFov(fov * math::util::DEG_2_RAD);
  }
  ImGui::End();

  if (ImGui::Begin("Player")) {
    math::Vector3 position = player_.GetPosition();
    math::Vector3 velocity = player_.GetVelocity();
    float impulse = player_.GetImpulse();
    ImGui::SliderFloat3("Velocity", &velocity.x, -1.0f, 1.0f);
    ImGui::SliderFloat("Impulse", &impulse, 0, 1.0f);
    ImGui::SliderFloat3("Position", &position.x, -100.0f, 100.0f);

    math::Vector3 rotation = math::Quaternion::ToEular(player_.GetRotation()) *
                             math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &rotation.x, -180.0f, 180.0f);
    player_.SetRotation(
        math::Quaternion::FromEular(rotation * math::util::DEG_2_RAD));
  }
  ImGui::End();

  if (physics::Collision::GetInstance()->Collision_OBB_DeskOBB(
          player_.GetCollisionRef(), desk_.GetCollisionRef())) {
    MY_LOG(L"�����S���Ɗ����Փ˂��܂���");
    player_.SetPosition(player_.GetCollisionRef().GetPosition());
  }

  return true;
}

//�`��
void PlayerMoveViewer::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  device.GetHeapManager().SetCommandList(command_list);
  auto& render_resource_manager = device.GetRenderResourceManager();
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  camera_.RenderStart();
  player_.Draw();
  desk_.Draw();

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::NONE, false);
  device.GetHeapManager().SetHeapTableToGraphicsCommandList(device, command_list);
  player_.GetCollisionRef().DebugDraw(command_list);
  desk_.GetCollisionRef().DebugDraw(command_list);
}

//�I��
void PlayerMoveViewer::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend