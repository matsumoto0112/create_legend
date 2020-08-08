#include "src/scenes/scene_manager.h"

#include "src/scenes/debugscene/enemy_move_viewer.h"
#include "src/scenes/debugscene/model_view.h"
#include "src/scenes/debugscene/multi_render_target_test.h"
#include "src/scenes/debugscene/physics_test.h"
#include "src/scenes/debugscene/player_move_viewer.h"
#include "src/scenes/debugscene/sound_test.h"
#include "src/scenes/debugscene/sprite_render_test.h"
#include "src/scenes/debugscene/stage_generate_test.h"
#include "src/scenes/game_over.h"
#include "src/scenes/mainscene/main_scene_1.h"
#include "src/scenes/title.h"

namespace legend {
namespace scenes {

//コンストラクタ
SceneManager::SceneManager() : next_scene_(SceneType::NONE) {
  //シーン遷移は現状、この方法でしか分からない
  current_scene_ = std::make_unique<Title>(this);
  current_scene_type_ = SceneType::TITLE;
}

//初期化
bool SceneManager::Initialize() {
  if (current_scene_ == nullptr) {
    MY_LOG(L"シーンが設定されていません");
    return false;
  }

  if (!current_scene_->Initialize()) {
    MY_LOG(L"初期化に失敗しました");
    return false;
  }

  return true;
}

//終了
void SceneManager::Finalize() {
  if (current_scene_ == nullptr) {
    return;
  }

  current_scene_->Finalize();
}

//更新
bool SceneManager::Update() {
  if (current_scene_ == nullptr) {
    MY_LOG(L"シーンが設定されていません");
    return false;
  }

  if (next_scene_ != SceneType::NONE) {
    current_scene_->Finalize();

    auto CreateScene = [&](SceneType type) -> std::unique_ptr<Scene> {
      switch (type) {
        case SceneType::TITLE:
          return std::make_unique<Title>(this);
        case SceneType::GAMEOVER:
          return std::make_unique<GameOver>(this);
        case SceneType::MODEL_VIEW:
          return std::make_unique<debugscene::ModelView>(this);
        case SceneType::SOUND_TEST:
          return std::make_unique<debugscene::SoundTest>(this);
        case SceneType::PHYSICS_TEST:
          return std::make_unique<debugscene::PhysicsTest>(this);
        case SceneType::SPRITE_TEST:
          return std::make_unique<debugscene::SpriteRenderTest>(this);
        case SceneType::MULTI_RENDER_TARGET_TEST:
          return std::make_unique<debugscene::MultiRenderTargetTest>(this);
        case SceneType::PLAYER_MOVE_VIEWER:
          return std::make_unique<debugscene::PlayerMoveViewer>(this);
        case SceneType::ENEMY_MOVE_VIEWER:
          return std::make_unique<debugscene::EnemyMoveViewer>(this);
        case SceneType::MAIN_SCENE_1:
          return std::make_unique<mainscene::MainScene1>(this);
        case SceneType::STAGE_GENERATE_TEST:
          return std::make_unique<debugscene::StageGenerateTest>(this);
        default:
          MY_ASSERTION(false, L"存在しないシーンが選択されました。");
          return nullptr;
      }
    };

    current_scene_ = CreateScene(next_scene_);

    current_scene_type_ = next_scene_;
    next_scene_ = SceneType::NONE;
    if (!current_scene_->Initialize()) {
      return false;
    }
  }

  if (!current_scene_->Update()) {
    MY_LOG(L"更新に失敗しました");
    return false;
  }

  return true;
}

//描画
void SceneManager::Draw() {
  if (current_scene_ == nullptr) {
    return;
  }

  current_scene_->Draw();
}

//シーン遷移
void SceneManager::ChangeScene(SceneType next_scene) {
  next_scene_ = next_scene;
}

//現在のシーンの取得
SceneType SceneManager::GetCurrentSceneType() const {
  return current_scene_type_;
}
}  // namespace scenes
}  // namespace legend
