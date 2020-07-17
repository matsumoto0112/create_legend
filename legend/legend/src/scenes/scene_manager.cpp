#include "src/scenes/scene_manager.h"

#include "src/scenes/debugscene/model_view.h"
#include "src/scenes/debugscene/perspective_camera_test.h"
#include "src/scenes/debugscene/physics_test.h"
#include "src/scenes/debugscene/post_process_viewer.h"
#include "src/scenes/debugscene/sound_test.h"
#include "src/scenes/debugscene/sprite_render_test.h"
#include "src/scenes/game_over.h"
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

    //シーン遷移は現状、この方法でしか分からない
    switch (next_scene_) {
      case SceneType::TITLE:
        current_scene_ = std::make_unique<Title>(this);
        break;
      case SceneType::GAMEOVER:
        current_scene_ = std::make_unique<GameOver>(this);
        break;
      case SceneType::MODEL_VIEW:
        current_scene_ = std::make_unique<debugscene::ModelView>(this);
        break;
      case SceneType::SOUND_TEST:
        current_scene_ = std::make_unique<debugscene::SoundTest>(this);
        break;
      case SceneType::PERSPECTIVE_CAMERA_TEST:
        current_scene_ =
            std::make_unique<debugscene::PerspectiveCameraTest>(this);
        break;
      case SceneType::PHYSICS_TEST:
        current_scene_ = std::make_unique<debugscene::PhysicsTest>(this);
        break;
      case SceneType::SPRITE_TEST:
        current_scene_ = std::make_unique<debugscene::SpriteRenderTest>(this);
        break;
      case SceneType::POST_PROCES_VIEWER:
        current_scene_ = std::make_unique<debugscene::PostProcessViewer>(this);
        break;
      default:
        MY_ASSERTION(false, L"存在しないシーンが選択されました。");
        break;
    }

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
