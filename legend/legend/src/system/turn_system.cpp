#include "src/system/turn_system.h"

namespace legend {
namespace system {

//�R���X�g���N�^
TurnSystem::TurnSystem() : current_turn_(0) {}

//�f�X�g���N�^
TurnSystem::~TurnSystem() {}

//������
bool TurnSystem::Init(const std::string& stage_name) {
  //�X�e�[�W�f�[�^�̊g���q��.txt
  auto stage_path = util::Path::GetInstance()->exe() / "assets" / "stage" /
                    (stage_name + ".txt");

  if (!physics_field_.Init()) {
    return false;
  }
  if (!enemy_manager_.Initilaize()) {
    return false;
  }

  if (!stage_generator_.LoadStage(stage_path, stage_name, &physics_field_,
                                  &desks_, &obstacles_, &player_,
                                  &enemy_manager_)) {
    return false;
  }

  physics_field_.SetPlayer(player_.GetCollisionRef());

  player_.SetMediator(this);
  for (auto&& desk : desks_) {
    physics_field_.AddDesk(desk.GetCollisionRef());
    desk.SetMediator(this);
  }
  for (auto&& obs : obstacles_) {
    physics_field_.AddObstacle(obs.GetCollisionRef());
    obs.SetMediator(this);
  }

  //�J�����̏�����
  {
    const math::Vector3 camera_position = math::Vector3(0, 50.0f, -50.0f);
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
    if (!main_camera_.Init(L"MainCamera", &player_,
                           math::Vector3(0.0f, 30.0f, -30.0f), camera_rotation,
                           math::util::DEG_2_RAD * 50.0f, aspect_ratio,
                           math::Vector3::kUpVector, 0.1f, 300.0f)) {
      return false;
    }
  }

  return true;
}

bool TurnSystem::Update() {
  countdown_timer_.Update();
  player_.Update();

  const std::unordered_map<Mode, std::function<bool()>> switcher = {
      {Mode::PLAYER_MOVE_READY, [&]() { return PlayerMoveReady(); }},
      {Mode::PLAYER_MOVING, [&]() { return PlayerMoving(); }},
      {Mode::PLAYER_SKILL_AFTER_MOVED,
       [&]() { return PlayerSkillAfterModed(); }},
      {Mode::ENEMY_MOVING, [&]() { return EnemyMove(); }},
      {Mode::ENEMY_MOVE_END, [&]() { return EnemyMoveEnd(); }},
  };
  if (!switcher.at(current_mode_)()) {
    return false;
  }

  //���������̓K�p
  if (!physics_field_.Update(current_mode_, player_.GetVelocity(),
                             player_.GetIsMove(), player_.GetImpulse(),
                             player_.GetPower(), enemy_manager_.GetVelocities(),
                             !enemy_manager_.LastEnemyMoveEnd())) {
    return false;
  }

  //���������̌��ʂ��e�I�u�W�F�N�g�ɍX�V������
  player_.SetPosition(physics_field_.GetPlayerOBB().GetPosition());
  player_.SetVelocity(physics_field_.GetPlayerVelocity());
  player_.SetRotation(physics_field_.GetPlayerOBB().GetRotation());
  enemy_manager_.SetPosition(physics_field_);
  enemy_manager_.SetVelocity(physics_field_);

  if (ImGui::Begin("Camera")) {
    //�J�������W
    math::Vector3 camera_position = main_camera_.GetPosition();
    ImGui::SliderFloat3("Position", &camera_position.x, -100.0f, 100.0f);
    main_camera_.SetPosition(camera_position);
    //�J������]�p
    math::Vector3 camera_rotation =
        math::Quaternion::ToEular(main_camera_.GetRotation()) *
        math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &camera_rotation.x, -180.0f, 180.0f);
    main_camera_.SetRotation(
        math::Quaternion::FromEular(camera_rotation * math::util::DEG_2_RAD));

    //�J�����̏�����x�N�g����ύX����
    if (ImGui::Button("X_UP")) {
      main_camera_.SetUpVector(math::Vector3::kRightVector);
    }
    if (ImGui::Button("Y_UP")) {
      main_camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("Z_UP")) {
      main_camera_.SetUpVector(math::Vector3::kForwardVector);
    }
    float fov = main_camera_.GetFov() * math::util::RAD_2_DEG;
    ImGui::SliderFloat("FOV", &fov, 0.01f, 90.0f);
    main_camera_.SetFov(fov * math::util::DEG_2_RAD);

    if (ImGui::Button("BackCamera")) {
      main_camera_.SetPosition(math::Vector3(0, 50.0f, -50.0f));
      main_camera_.SetRotation(math::Quaternion::FromEular(
          math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f));
      main_camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("RightCamera")) {
      main_camera_.SetPosition(math::Vector3(50.0f, 5.0f, 0));
      main_camera_.SetRotation(math::Quaternion::FromEular(
          0.0f, math::util::DEG_2_RAD * -90.0f, 0.0f));
      main_camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("UpCamera")) {
      main_camera_.SetPosition(math::Vector3(0, 100.0f, 0));
      main_camera_.SetRotation(math::Quaternion::FromEular(
          math::util::DEG_2_RAD * 90.0f, 0.0f, 0.0f));
      main_camera_.SetUpVector(math::Vector3::kForwardVector);
    }
  }
  ImGui::End();

  return true;
}

//�v���C���[�̈ړ�����
bool TurnSystem::PlayerMoveReady() {
  //�v���C���[�̑��x�X�V�͓��͂��󂯎���ď�������
  player_.SetVelocity();
  player_.SetImpulse();
  return true;
}

//�v���C���[�̈ړ�������
bool TurnSystem::PlayerMoving() { return true; }

bool TurnSystem::PlayerSkillAfterModed() {
  current_mode_ = Mode::ENEMY_MOVING;
  return true;
}

//�G�̈ړ�����
bool TurnSystem::EnemyMove() {
  MY_LOG(L"EnemyMove");
  enemy_manager_.Update(nullptr);
  enemy_manager_.SetPlayer(player_.GetCollisionRef());
  if (enemy_manager_.GetEnemiesSize() == 0 ||
      enemy_manager_.LastEnemyMoveEnd()) {
    current_mode_ = Mode::ENEMY_MOVE_END;
    physics_field_.ResetEnemyMove();
    AddCurrentTurn();
  }
  return true;
}

//�G�̈ړ��I��������
bool TurnSystem::EnemyMoveEnd() {
  current_mode_ = Mode::PLAYER_MOVE_READY;
  return true;
}

//�`��
void TurnSystem::Draw() {
  main_camera_.RenderStart();
  player_.Draw();
  for (auto&& desk : desks_) {
    desk.Draw();
  }
  enemy_manager_.Draw();
  for (auto&& obs : obstacles_) {
    obs.Draw();
  }
}

//�f�o�b�O�`��
void TurnSystem::DebugDraw() {
  auto& command_list = game::GameDevice::GetInstance()
                           ->GetDevice()
                           .GetCurrentFrameResource()
                           ->GetCommandList();

  player_.GetCollisionRef().DebugDraw(command_list);
  for (auto&& desk : desks_) {
    desk.GetCollisionRef().DebugDraw(command_list);
  }
  enemy_manager_.DebugDraw(command_list);
  for (auto&& obs : obstacles_) {
    obs.GetCollisionRef().DebugDraw(command_list);
  }
}

//�^�[�����̑���
void TurnSystem::AddCurrentTurn() { current_turn_++; }

//���݂̃^�[�������擾
i32 TurnSystem::GetCurrentTurn() { return current_turn_; }

//�v���C���[�̈ړ��J�n������
void TurnSystem::PlayerMoveStartEvent() { current_mode_ = Mode::PLAYER_MOVING; }

//�v���C���[�̈ړ��I��������
void TurnSystem::PlayerMoveEndEvent() {
  player_.ResetMoveEnd();
  // 0.1�b��Ƀ��[�h��؂�ւ���
  countdown_timer_.Init(
      0.1f, [&]() { current_mode_ = Mode::PLAYER_SKILL_AFTER_MOVED; });
}

//�v���C���[�̃X�L������������
void TurnSystem::PlayerSkillActivate() {}

//�v���C���[�̃X�L�������I��������
void TurnSystem::PlayerSkillDeactivate() {}

player::Player* TurnSystem::GetPlayer() { return &player_; }

std::vector<enemy::Enemy*> TurnSystem::GetEnemies() {
  return enemy_manager_.GetEnemyPointers();
}

}  // namespace system
}  // namespace legend