#include "src/scenes/scene_manager.h"

#include "src/scenes/title.h"
#include "src/scenes/game_over.h"

namespace legend {
namespace scenes {

//�R���X�g���N�^
SceneManager::SceneManager() : next_scene_(SceneType::NONE) {
  //�V�[���J�ڂ͌���A���̕��@�ł���������Ȃ�
  current_scene_ = static_cast<Scene*>(new Title(this));
}

//������
void SceneManager::Initialize() {
  if (current_scene_ == nullptr) {
    return;
  }

  current_scene_->Initialize();
}

//�I��
void SceneManager::Finalize() {
  if (current_scene_ == nullptr) {
    return;
  }

  current_scene_->Finalize();
}

//�X�V
void SceneManager::Update() {
  if (current_scene_ == nullptr) {
    return;
  }

  if (next_scene_ != SceneType::NONE) {
    current_scene_->Finalize();
    delete current_scene_;

    //�V�[���J�ڂ͌���A���̕��@�ł���������Ȃ�
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

//�`��
void SceneManager::Draw() {
  if (current_scene_ == nullptr) {
    return;
  }

  current_scene_->Draw();
}

//�V�[���J��
void SceneManager::ChangeScene(SceneType next_scene) {
  next_scene_ = next_scene;
}
}  // namespace scenes
}  // namespace legend
