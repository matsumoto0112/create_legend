#include "src/scenes/debugscene/stage_generate_test.h"

#include "src/game/game_device.h"
#include "src/util/path.h"
#include "src/util/string_util.h"

namespace legend {
namespace scenes {
namespace debugscene {
StageGenerateTest::StageGenerateTest(ISceneChange* scene_change)
    : Scene(scene_change) {}
StageGenerateTest::~StageGenerateTest() {}
bool StageGenerateTest::Initialize() {
  indexs_.clear();
  indexs_.push_back("no data");

  //physics_field_.Init();

  //�J�����̏�����
  const math::Vector3 camera_position = math::Vector3(0, 40, -40);
  const math::Quaternion camera_rotation =
      math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
  const math::IntVector2 screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
  if (!camera_.Init(L"MainCamera", camera_position, camera_rotation,
                    math::util::DEG_2_RAD * 50.0f, aspect_ratio)) {
    MY_LOG(L"�J�����̏������Ɏ��s���܂����B");
  }

  //�v���C���[�̏�����
  {
    player::Player::InitializeParameter player_parameter;
    player_parameter.transform =
        util::Transform(math::Vector3(0, 0.1f, 0), math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    player_parameter.bouding_box_length =
        math::Vector3(0.06f, 0.025f, 0.14f) / 4.0f;
    player_parameter.min_power = 0;
    player_parameter.max_power = 1;
    if (!player_.Init(nullptr, player_parameter)) {
      // return false;
    }
    // physics_field_.SetPlayer(player_.GetCollisionRef());
  }
  return true;
}
bool StageGenerateTest::Update() {
  //�X�e�[�W�I��
  if (ImGui::Begin("StageGenerateTest")) {
    ImGui::Text("Currently closed");
    // if (ImGui::Button("Load aaaa")) {
    //  indexs_ = stage_generator_->LoadStringStageData(
    //      util::Path::GetInstance()->exe() / L"assets" / L"stage" /
    //      L"aaaa.txt");
    //  stage_generator_->LoadStage(
    //      util::Path::GetInstance()->exe() / L"assets" / L"stage" /
    //      L"aaaa.txt", "aaaa", &desks_, &obstacles_, &player_);
    //}
    // if (ImGui::Button("Load bbbb")) {
    //  indexs_ = stage_generator_->LoadStringStageData(
    //      util::Path::GetInstance()->exe() / L"assets" / L"stage" /
    //      L"bbbb.txt");
    //  stage_generator_->LoadStage(
    //      util::Path::GetInstance()->exe() / L"assets" / L"stage" /
    //      L"bbbb.txt", "bbbb", &desks_, &obstacles_, &player_);
    //}
  }
  ImGui::End();

  //�J�����̐ݒ�
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

    if (ImGui::Button("BackCamera")) {
      camera_.SetPosition(math::Vector3(0, 40, -40));
      camera_.SetRotation(math::Quaternion::FromEular(
          math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f));
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("RightCamera")) {
      camera_.SetPosition(math::Vector3(50.0f, 0.05f, 0));
      camera_.SetRotation(math::Quaternion::FromEular(
          0.0f, math::util::DEG_2_RAD * -90.0f, 0.0f));
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("UpCamera")) {
      camera_.SetPosition(math::Vector3(0, 50.0f, 0));
      camera_.SetRotation(math::Quaternion::FromEular(
          math::util::DEG_2_RAD * 90.0f, 0.0f, 0.0f));
      camera_.SetUpVector(math::Vector3::kForwardVector);
    }
  }
  ImGui::End();

  //�ǂݍ��񂾃X�e�[�W���
  if (ImGui::Begin("LoadData")) {
    for (auto&& index : indexs_) {
      ImGui::Text(index.c_str());
    }
  }
  ImGui::End();
  return true;
}
void StageGenerateTest::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  device.GetRenderResourceManager().SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  auto& render_resource_manager = device.GetRenderResourceManager();
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  camera_.RenderStart();

  // for (auto object : map_actors_) {
  //    object.Draw();
  //}
  player_.Draw();
  enemy_manager_.Draw();
  for (auto object : desks_) {
    object.Draw();
  }
  for (auto object : obstacles_) {
    object.Draw();
  }

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::NONE, false);
  device.GetHeapManager().SetHeapTableToGraphicsCommandList(device,
                                                            command_list);

  // for (auto object : map_actors_) {
  //    object.GetCollisionRef().DebugDraw(command_list);
  //}
  // player_.GetCollisionRef().DebugDraw(command_list);
  // for (auto&& obs : desks_) {
  //  obs.GetCollisionRef().DebugDraw(command_list);
  //}
  // for (auto&& obs : obstacles_) {
  //  obs.GetCollisionRef().DebugDraw(command_list);
  //}
}
void StageGenerateTest::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
