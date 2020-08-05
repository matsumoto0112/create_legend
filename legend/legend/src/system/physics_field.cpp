#include "src/system/physics_field.h"

#include "src/physics/collision.h"

namespace legend {
namespace system {

//�R���X�g���N�^
PhysicsField::PhysicsField() : turn_(Turn::PLAYER_TURN), gravity_(-9.8f) {}

//�f�X�g���N�^
PhysicsField::~PhysicsField() {}

//�v���C���[�̏�����
bool PhysicsField::PlayerInit(player::Player::InitializeParameter parameter) {
  if (!player_.Init(parameter)) {
    return false;
  }

  return true;
}

//�G�l�~�[�̏�����
// bool PhysicsField::EnemYyInit(�G�l�~�[::InitializeParameter parameter) {
//  �G�l�~�[ enemy;
//  if (!enemy.Init(parameter)) {
//    return false;
//  }
//
//  AddEnemy(enemy);
//  return true;
//}

//���̏�����
bool PhysicsField::DeskInit(object::Desk::InitializeParameter parameter) {
  object::Desk desk;
  if (!desk.Init(parameter)) {
    return false;
  }

  AddDesk(desk);
  return true;
}

//�X�V
bool PhysicsField::Update() {
  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  switch (turn_) {
    case legend::system::Turn::PLAYER_TURN:
      if (!player_.Update()) {
        return false;
      }
      if (player_.GetMoveEnd()) {
        turn_ = system::Turn::ENEMY_TURN;
        player_.ResetMoveEnd();
      }
      MY_LOG(L"PLAYERTURN");
      break;
    case legend::system::Turn::ENEMY_TURN:
      if (input.GetGamepad()->GetButtonDown(input::joy_code::X)) {
        turn_ = system::Turn::PLAYER_TURN;
      }
      MY_LOG(L"ENEMYTURN");
      break;
    default:
      break;
  }

  if (!player_.GetCollisionRef().GetOnGround()) {
    player_.UpdateGravity(gravity_);
  }

  //�v���C���[�Ɗe���Ƃ̏Փ˔���𒲂ׂ�
  for (i32 i = 0; i < desks_.size(); i++) {
    if (physics::Collision::GetInstance()->Collision_OBB_DeskOBB(
            player_.GetCollisionRef(), desks_[i].GetCollisionRef())) {
      MY_LOG(L"�v���C���[�����S���Ɗ����Փ˂��܂���");
      player_.SetPosition(player_.GetCollisionRef().GetPosition());
      break;
    }
  }

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

  return true;
}

void PhysicsField::Draw() {
  player_.Draw();
  for (i32 i = 0; i < desks_.size(); i++) {
    desks_[i].Draw();
  }
  // for (i32 i = 0; i < enemies_.size(); i++) {
  //    enemies_[i].Draw();
  //}
}

////�G�l�~�[�̓o�^
// void PhysicsField::AddEnemy(�G�l�~�[ enemy)
//{
//    enemies_.push_back(enemy);
//}

//���̓o�^
void PhysicsField::AddDesk(object::Desk desk) { desks_.push_back(desk); }
}  // namespace system
}  // namespace legend