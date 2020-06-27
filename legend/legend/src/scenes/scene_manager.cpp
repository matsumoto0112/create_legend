#include "src/scenes/scene_manager.h"

#include "src/scenes/title.h"
#include "src/scenes/game_over.h"

namespace legend {
namespace scenes {

//コンストラクタ
SceneManager::SceneManager() : next_scene_(SceneType::NONE) {
  //シーン遷移は現状、この方法でしか分からない
  current_scene_ = static_cast<Scene*>(new Title(this));
}

//初期化
void SceneManager::Initialize() {
  if (current_scene_ == nullptr) {
    return;
  }

  current_scene_->Initialize();
}

//終了
void SceneManager::Finalize() {
  if (current_scene_ == nullptr) {
    return;
  }

  current_scene_->Finalize();
}

//更新
void SceneManager::Update() {
  if (current_scene_ == nullptr) {
    return;
  }

  if (next_scene_ != SceneType::NONE) {
    current_scene_->Finalize();
    delete current_scene_;

    //シーン遷移は現状、この方法でしか分からない
    switch (next_scene_) {
      case SceneType::TITLE:
        current_scene_ = static_cast<Scene*>(new Title(this));
        break;
      case SceneType::GAMEOVER:
          current_scene_ = static_cast<Scene*>(new GameOver(this));
          break;
      default:
        break;
    }

    next_scene_ = SceneType::NONE;
    current_scene_->Initialize();
  }

  current_scene_->Update();
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
}  // namespace scenes
}  // namespace legend
