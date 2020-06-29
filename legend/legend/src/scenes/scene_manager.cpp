#include "src/scenes/scene_manager.h"

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

    //シーン遷移は現状、この方法でしか分からない
    switch (next_scene_) {
      case SceneType::TITLE:
        current_scene_ = std::make_unique<Title>(this);
        current_scene_type_ = next_scene_;
        break;
      case SceneType::GAMEOVER:
        current_scene_ = std::make_unique<GameOver>(this);
        current_scene_type_ = next_scene_;
        break;
      default:
        MY_ASSERTION(false, L"存在しないシーンが選択されました。");
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

//現在のシーンの取得
SceneType SceneManager::GetCurrentSceneType() const {
  return current_scene_type_;
}
}  // namespace scenes
}  // namespace legend
