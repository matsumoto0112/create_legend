#include "src/scenes/mainscene/main_scene_1.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/game/game_device.h"

namespace legend {
namespace scenes {
namespace mainscene {

//�R���X�g���N�^
MainScene1::MainScene1(ISceneChange* scene_change) : Scene(scene_change) {}

//�f�X�g���N�^
MainScene1::~MainScene1() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}

//������
bool MainScene1::Initialize() {
  turn_ = system::Turn::PLAYER_TURN;
  current_turn_ = system::TurnSystem();
  physics_field_.Init();

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

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
  const std::filesystem::path player_model_path =
      util::Path::GetInstance()->model() / "eraser_01.glb";
  if (!resource.GetModel().Load(util::resource::ModelID::ERASER,
                                player_model_path)) {
    return false;
  }
  const std::filesystem::path desk_model_path =
      util::Path::GetInstance()->model() / "desk.glb";
  if (!resource.GetModel().Load(util::resource::ModelID::DESK,
                                desk_model_path)) {
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

  //�v���C���[�̏�����
  {
    player::Player::InitializeParameter player_parameter;
    player_parameter.transform =
        util::Transform(math::Vector3::kZeroVector, math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    player_parameter.bouding_box_length = math::Vector3(1.0f, 0.5f, 2.0f);
    player_parameter.min_power = 0;
    player_parameter.max_power = 1;
    if (!player_.Init(player_parameter)) {
      return false;
    }
    physics_field_.SetPlayer(player_.GetCollisionRef());
  }

  //���̏�����
  {
    //�{���̓X�e�[�W�f�[�^����ǂݍ���
    object::Desk::InitializeParameter desk_parameter;
    desk_parameter.transform =
        util::Transform(math::Vector3::kZeroVector, math::Quaternion::kIdentity,
                        math::Vector3::kUnitVector);
    desk_parameter.bounding_box_length = math::Vector3(10.0f, 0.5f, 10.0f);
    desk_parameter.normal = math::Vector3::kUpVector;
    if (!desk_.Init(desk_parameter)) {
      return false;
    }
    physics_field_.AddDesk(desk_.GetCollisionRef());
  }

  if (!enemy_manager_.Initilaize(&physics_field_)) {
    return false;
  }

  //�J�����̏�����
  {
    const math::Vector3 camera_position = math::Vector3(0, 0.5f, -0.5f);
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
bool MainScene1::Update() {
  if (!UpdateTurn()) {
    return false;
  }
  if (!physics_field_.Update(turn_, player_.GetVelocity(), player_.GetIsMove(),
                             player_.GetImpulse(), player_.GetPower(),
                             enemy_manager_.GetVelocities(),
                             !enemy_manager_.LastEnemyMoveEnd())) {
    return false;
  }

  player_.SetPosition(physics_field_.GetPlayerOBB().GetPosition());
  enemy_manager_.SetPosition(&physics_field_);

  player_.SetVelocity(physics_field_.GetPlayerVelocity());
  enemy_manager_.SetVelocity(&physics_field_);

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

  return true;
}

//�`��
void MainScene1::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  device.GetRenderResourceManager().SetDepthStencilTargetID(
      directx::render_target::DepthStencilTargetID::Depth);
  device.GetRenderResourceManager().SetRenderTargetsToCommandList(device);
  device.GetRenderResourceManager().ClearCurrentDepthStencilTarget(device);

  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(util::resource::id::Pipeline::MODEL_VIEW)
      ->SetGraphicsCommandList(device);
  camera_.RenderStart();
  player_.Draw();
  desk_.Draw();
  enemy_manager_.Draw();
}

//�I��
void MainScene1::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();

  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  resource.GetVertexShader().Unload(
      util::resource::id::VertexShader::MODEL_VIEW);
  resource.GetPixelShader().Unload(util::resource::id::PixelShader::MODEL_VIEW);
  resource.GetPipeline().Unload(util::resource::id::Pipeline::MODEL_VIEW);
  resource.GetModel().Unload(util::resource::ModelID::ERASER);
  resource.GetModel().Unload(util::resource::ModelID::DESK);
}

//�^�[���ʂ̍X�V����
bool MainScene1::UpdateTurn() {
  switch (turn_) {
    case legend::system::Turn::PLAYER_TURN:
      if (!player_.Update()) {
        return false;
      }
      //�v���C���[�������I������^�[���؂�ւ�
      if (player_.GetMoveEnd()) {
        turn_ = system::Turn::ENEMY_TURN;
        player_.ResetMoveEnd();
      }
      MY_LOG(L"PLAYER TURN");
      break;
    case legend::system::Turn::ENEMY_TURN:
      if (!enemy_manager_.Update(&physics_field_)) {
        return false;
      }
      //�Ō�ɓo�^����Ă���G�l�~�[�������I�����疔�̓G�l�~�[���S�Ă��Ȃ���΁A�^�[���؂�ւ�
      //�^�[�������Z
      if (enemy_manager_.LastEnemyMoveEnd() ||
          enemy_manager_.GetEnemiesSize() == 0) {
        turn_ = system::Turn::PLAYER_TURN;
        current_turn_.AddCurrentTurn();
        physics_field_.ResetEnemyMove();
      }
      MY_LOG(L"ENEMY TURN");
      break;
    default:
      break;
  }
  return true;
}
}  // namespace mainscene
}  // namespace scenes
}  // namespace legend