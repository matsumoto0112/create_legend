#include "actor_manager.h"

#include "src/system/turn_system.h"

namespace legend {
namespace actor {
//�R���X�g���N�^
ActorManager::ActorManager() {}

//�f�X�g���N�^
ActorManager::~ActorManager() { static_actors_.clear(); }

bool ActorManager::Init(const std::string& stage_name,
                        system::TurnSystem* turn_system) {
  turn_system_ = turn_system;

  hit_stop_time_ = 0.0f;

  //�X�e�[�W�f�[�^�̊g���q��.txt
  auto stage_path = util::Path::GetInstance()->exe() / "assets" / "stage" /
                    (stage_name + ".txt");

  //�����̏�����
  if (!physics_field_.Init()) {
    return false;
  }

  //�G�l�~�[�Ǘ��N���X�̏�����
  if (!enemy_manager_.Initilaize(this)) {
    return false;
  }

  //�X�e�[�W�f�[�^��ǂݍ���
  stage_generator_.LoadStringStageData(stage_path, stage_name);
  if (!GenerateActors(0)) {
    return false;
  }

  // AI�̕���_�̓ǂݍ���
  search_manager_.Initialize(this);
  //�T���f�[�^�̊g���q��.txt
  auto search_path = util::Path::GetInstance()->exe() / "assets" / "stage" /
                     (stage_name + "_searchData" + ".txt");
  search_manager_.Make(search_path);

  return true;
}

bool ActorManager::Update() {
  if (hit_stop_time_ > 0.0f) {
    float delta_time = game::GameDevice::GetInstance()
                           ->GetFPSCounter()
                           .GetDeltaSeconds<float>();
    hit_stop_time_ -= delta_time;
    return true;
  }
  hit_stop_time_ = 0.0f;

  countdown_timer_.Update();

  player_->Update();
  enemy_manager_.DestroyUpdate();

  for (auto&& fragment : alpha_actors_) {
    fragment->Update();
  }
  for (auto&& item_box : static_actors_) {
    item_box->Update();
  }

  alpha_actors_.erase(
      std::remove_if(alpha_actors_.begin(), alpha_actors_.end(),
                     [&](auto& it) {
                       return remove_alpha_actors_.find(it.get()) !=
                              remove_alpha_actors_.end();
                     }),
      alpha_actors_.end());
  remove_alpha_actors_.clear();

  static_actors_.erase(
      std::remove_if(static_actors_.begin(), static_actors_.end(),
                     [&](auto& it) {
                       return remove_static_actors_.find(it.get()) !=
                              remove_static_actors_.end();
                     }),
      static_actors_.end());
  remove_static_actors_.clear();

  physics_field_.Update();

  return true;
}

void ActorManager::DrawDifferedRenderingObject(
    camera::Camera* camera, directx::device::CommandList& command_list) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& render_resource_manager = device.GetRenderResourceManager();

  render_resource_manager.SetRenderTargets(
      command_list,
      directx::render_target::RenderTargetID::DIFFERED_RENDERING_PRE, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);
  camera->RenderStart();

  actor_render_command_list_.Push(player_.get());
  for (auto&& obj : static_actors_) {
    actor_render_command_list_.Push(obj.get());
  }
  for (auto&& fragment : alpha_actors_) {
    actor_render_command_list_.Push(fragment.get());
  }
  enemy_manager_.Draw(&actor_render_command_list_);

  actor_render_command_list_.RenderPass();
}

void ActorManager::DrawAlphaObject(directx::device::CommandList& command_list) {
  for (auto&& actor : alpha_actors_) {
    actor->Draw();
  }
}

void ActorManager::Draw2D(directx::device::CommandList& command_list) {}

void ActorManager::DrawEnd() { actor_render_command_list_.Clear(); }

void ActorManager::DebugDraw(camera::Camera* camera) {
  search_manager_.DebugDraw(&physics_field_);
  physics_field_.DebugDraw(camera);
}

void ActorManager::PlayerMoveStartEvent() {
  turn_system_->SetTurnMode(system::Mode::PLAYER_MOVING);
  turn_system_->SetCameraMode(system::camera_mode::Sub1);
}

void ActorManager::PlayerMoveEndEvent() {
  turn_system_->SetTurnMode(system::Mode::PLAYER_MOVE_END);
  turn_system_->SetCameraMode(system::camera_mode::Main);
}

void ActorManager::PlayerSkillActivate() {}

void ActorManager::PlayerSkillDeactivate() {}

player::Player* ActorManager::GetPlayer() const { return player_.get(); }

void ActorManager::AddFragment(std::unique_ptr<object::Fragment> fragment) {
  alpha_actors_.emplace_back(std::move(fragment));
}

bool ActorManager::GenerateActors(i32 currnt_turn) {
  {
    player::Player::InitializeParameter player;
    std::vector<object::Desk::InitializeParameter> desks;
    std::vector<object::Obstacle::InitializeParameter> obstacles;
    std::vector<object::GraffitiInitializeParameter> graffities;
    std::vector<skill::SkillItemBox::InitializeParameter> item_boxes;
    std::vector<enemy::Enemy::InitializeParameter> enemys;
    std::vector<enemy::Boss::InitializeParameter> bosses;
    if (!stage_generator_.GetMapActors(currnt_turn, player, desks, obstacles,
                                       graffities, item_boxes, enemys,
                                       bosses)) {
      return false;
    }

    if (!player_) {
      player_ = std::make_unique<player::Player>();
      if (!player_->Init(this, player)) {
        return false;
      }
    }
    for (auto&& param : desks) {
      auto obj = std::make_unique<object::Desk>();
      if (!obj->Init(this, param)) {
        return false;
      }
      static_actors_.emplace_back(std::move(obj));
    }

    for (auto&& param : obstacles) {
      auto obj = std::make_unique<object::Obstacle>();
      if (!obj->Init(this, param)) {
        return false;
      }
      static_actors_.emplace_back(std::move(obj));
    }
    directx::device::CommandList command_list;
    if (!command_list.Init(
            game::GameDevice::GetInstance()->GetDevice(),
            D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
      return false;
    }

    for (auto&& param : graffities) {
      auto graf = std::make_unique<object::Graffiti>();
      if (!graf->Init(this, param, command_list)) {
        return false;
      }
      static_actors_.emplace_back(std::move(graf));
    }

    command_list.Close();
    game::GameDevice::GetInstance()->GetDevice().ExecuteCommandList(
        {command_list});
    game::GameDevice::GetInstance()->GetDevice().WaitExecute();

    for (auto&& param : item_boxes) {
      auto obj = std::make_unique<skill::SkillItemBox>();
      std::shared_ptr<skill::Skill> skill =
          std::make_shared<skill::SkillPencil>();
      if (!obj->Init(this, param, skill)) {
        return false;
      }
      static_actors_.emplace_back(std::move(obj));
    }

    for (auto&& enemy_parameter : enemys) {
      enemy_manager_.AddEnemy(enemy_parameter);
    }

    for (auto&& boss_parameter : bosses) {
      enemy_manager_.AddBoss(boss_parameter);
    }
  }

  return true;
}

bool ActorManager::IsGameClear() const { return enemy_manager_.IsGameClear(); }

i32 legend::actor::ActorManager::GetEnemiesSize() {
  return enemy_manager_.GetEnemiesSize();
}

bool legend::actor::ActorManager::IsAllEnemeyStop() {
  for (auto&& velocity : enemy_manager_.GetVelocities()) {
    if (velocity != math::Vector3::kZeroVector) {
      return false;
    }
  }

  return true;
}

enemy::EnemyManager* legend::actor::ActorManager::GetEnemyManager() {
  return &enemy_manager_;
}

void legend::actor::ActorManager::EnemyManagerUpdate() {
  enemy_manager_.Update(&search_manager_);
  enemy_manager_.SetPlayer(player_->GetCollider());
}

std::vector<enemy::Enemy*> ActorManager::GetEnemies() {
  return enemy_manager_.GetEnemyPointers();
}

void ActorManager::AddCollider(std::shared_ptr<bullet::Collider> collider) {
  physics_field_.AddCollision(collider);
}

void ActorManager::RemoveCollider(std::shared_ptr<bullet::Collider> collider) {
  physics_field_.RemoveCollision(collider);
}

btCollisionWorld::AllHitsRayResultCallback ActorManager::RayCast(
    const math::Vector3& start, const math::Vector3& end) const {
  return physics_field_.RayCast(start, end);
}

void ActorManager::RemoveActor(actor::Actor* actor) {
  if (auto g = dynamic_cast<object::Graffiti*>(actor); g) {
    remove_alpha_actors_.emplace(g);
  }
  if (auto fragment = dynamic_cast<object::Fragment*>(actor); fragment) {
    remove_static_actors_.emplace(fragment);
  }
  if (auto item_box = dynamic_cast<skill::SkillItemBox*>(actor); item_box) {
    remove_static_actors_.emplace(item_box);
  }
}

float ActorManager::GetMainCameraThetaAngle() const {
  return turn_system_->GetPlayerFollowLookatCamera()->GetTheta();
}

system::Mode ActorManager::GetCurrentTurn() const { 
    return turn_system_->GetCurrentMode();
}

void ActorManager::AddStopTime(float time) {
  hit_stop_time_ += time;
}

void ActorManager::SetStopTime(float time) { hit_stop_time_ = time; }

}  // namespace actor
}  // namespace legend