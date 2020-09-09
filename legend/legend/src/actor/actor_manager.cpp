#include "actor_manager.h"

#include "src/system/turn_system.h"

namespace legend {
namespace actor {
//コンストラクタ
ActorManager::ActorManager() {}

//デストラクタ
ActorManager::~ActorManager() { static_actors_.clear(); }

bool ActorManager::Init(const std::string& stage_name,
                        system::TurnSystem* turn_system) {
  turn_system_ = turn_system;

  hit_stop_time_ = 0.0f;
  is_boss_generated_ = false;

  //ステージデータの拡張子は.txt
  auto stage_path = util::Path::GetInstance()->exe() / "assets" / "stage" /
                    (stage_name + ".txt");

  //物理の初期化
  if (!physics_field_.Init()) {
    return false;
  }

  //エネミー管理クラスの初期化
  if (!enemy_manager_.Initilaize(this)) {
    return false;
  }

  //ステージデータを読み込み
  stage_generator_.LoadStringStageData(stage_path, stage_name);
  if (!GenerateActors(0)) {
    return false;
  }

  // AIの分岐点の読み込み
  search_manager_.Initialize(this);
  //探索データの拡張子は.txt
  auto search_path = util::Path::GetInstance()->exe() / "assets" / "stage" /
                     (stage_name + "_searchData" + ".txt");
  search_manager_.Make(search_path);

  if (!camera_manager_.Init(this, stage_generator_.GetCameraGenerateInfos())) {
    return false;
  }

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

  for (auto&& actor : alpha_actors_) {
    actor->Update();
  }
  for (auto&& actor : static_actors_) {
    actor->Update();
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

  camera_manager_.UpdateCamera();

  for (auto&& actor : add_static_actors_) {
    static_actors_.emplace_back(std::move(actor));
  }
  add_static_actors_.clear();
  for (auto&& actor : add_alpha_actors_) {
    alpha_actors_.emplace_back(std::move(actor));
  }
  add_alpha_actors_.clear();

  return true;
}

void ActorManager::DrawDifferedRenderingObject(
    directx::device::CommandList& command_list) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& render_resource_manager = device.GetRenderResourceManager();

  render_resource_manager.SetRenderTargets(
      command_list,
      directx::render_target::RenderTargetID::DIFFERED_RENDERING_PRE, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);
  camera_manager_.RenderStart();

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

void ActorManager::DebugDraw() {
  camera_manager_.RenderStart();
  search_manager_.DebugDraw(&physics_field_);
  physics_field_.DebugDraw(camera_manager_.GetCurrentCamera());
}

void ActorManager::PlayerMoveStartEvent() {
  turn_system_->SetTurnMode(system::Mode::PLAYER_MOVING);
  camera_manager_.SetCameraMode(camera::camera_mode::FREE);
}

void ActorManager::PlayerMoveEndEvent() {
  turn_system_->SetTurnMode(system::Mode::PLAYER_MOVE_END);
  camera_manager_.SetCameraMode(camera::camera_mode::PLAYER_LOOKAT);
}

void ActorManager::PlayerSkillActivate() {}

void ActorManager::PlayerSkillDeactivate() {}

void ActorManager::PlayerCompleteEquipment() {
  if (GetBeforeTurn() == system::Mode::PLAYER_SKILL_AFTER_MOVED)
    turn_system_->ToEnemyTurn();
  else if (GetBeforeTurn() == system::Mode::ENEMY_MOVE_END)
    turn_system_->SetTurnMode(system::Mode::ENEMY_GENERATE);

  camera_manager_.SetUpdateEnable(true);
}

player::Player* ActorManager::GetPlayer() const { return player_.get(); }

void ActorManager::AddFragment(std::unique_ptr<object::Fragment> fragment) {
  add_static_actors_.emplace_back(std::move(fragment));
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
      std::shared_ptr<skill::Skill> skill;
      if (param.skill_model_num == 0) {
        skill = std::make_shared<skill::SkillPencil>();
      } else if (param.skill_model_num == 1) {
        skill = std::make_shared<skill::SkillPasteStick>();
      } else {
        skill = std::make_shared<skill::SkillCompass>();
      }
      if (!obj->Init(this, param, skill)) {
        return false;
      }
      alpha_actors_.emplace_back(std::move(obj));
    }

    for (auto&& enemy_parameter : enemys) {
      enemy_manager_.AddEnemy(enemy_parameter);
      turn_system_->SetTurnMode(system::Mode::ENEMY_PRODUCTION);
    }

    for (auto&& boss_parameter : bosses) {
      enemy_manager_.AddBoss(boss_parameter);
      is_boss_generated_ = true;
      turn_system_->SetTurnMode(system::Mode::ENEMY_PRODUCTION);
      // turn_system_->SetTurnMode(system::Mode::BOSS_PRODUCTION);
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

bool legend::actor::ActorManager::IsBossGenerated() {
  return is_boss_generated_;
}

i32 legend::actor::ActorManager::GetBossGenerateTurn()
{
    return stage_generator_.GetBossGenerateTurn();
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
  return camera_manager_.GetPlayerLookatCamera()->GetTheta();
}

system::Mode ActorManager::GetCurrentTurn() const {
  return turn_system_->GetCurrentMode();
}

system::Mode legend::actor::ActorManager::GetBeforeTurn() const {
  return turn_system_->GetBeforeMode();
}

void ActorManager::AddStopTime(float time) { hit_stop_time_ += time; }

void ActorManager::SetStopTime(float time) { hit_stop_time_ = time; }

}  // namespace actor
}  // namespace legend
