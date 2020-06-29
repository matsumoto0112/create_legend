#include "src/scenes/scene_manager.h"

#include "src/scenes/game_over.h"
#include "src/scenes/title.h"

namespace legend {
namespace scenes {

//�R���X�g���N�^
SceneManager::SceneManager() : next_scene_(SceneType::NONE) {
  //�V�[���J�ڂ͌���A���̕��@�ł���������Ȃ�
  current_scene_ = std::make_unique<Title>(this);
  current_scene_type_ = SceneType::TITLE;
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

    //�V�[���J�ڂ͌���A���̕��@�ł���������Ȃ�
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
        MY_ASSERTION(false, L"���݂��Ȃ��V�[�����I������܂����B");
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

//���݂̃V�[���̎擾
SceneType SceneManager::GetCurrentSceneType() const {
  return current_scene_type_;
}
}  // namespace scenes
}  // namespace legend
