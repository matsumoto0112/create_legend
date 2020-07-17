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

//�R���X�g���N�^
SceneManager::SceneManager() : next_scene_(SceneType::NONE) {
  //�V�[���J�ڂ͌���A���̕��@�ł���������Ȃ�
  current_scene_ = std::make_unique<Title>(this);
  current_scene_type_ = SceneType::TITLE;
}

//������
bool SceneManager::Initialize() {
  if (current_scene_ == nullptr) {
    MY_LOG(L"�V�[�����ݒ肳��Ă��܂���");
    return false;
  }

  if (!current_scene_->Initialize()) {
    MY_LOG(L"�������Ɏ��s���܂���");
    return false;
  }

  return true;
}

//�I��
void SceneManager::Finalize() {
  if (current_scene_ == nullptr) {
    return;
  }

  current_scene_->Finalize();
}

//�X�V
bool SceneManager::Update() {
  if (current_scene_ == nullptr) {
    MY_LOG(L"�V�[�����ݒ肳��Ă��܂���");
    return false;
  }

  if (next_scene_ != SceneType::NONE) {
    current_scene_->Finalize();

    //�V�[���J�ڂ͌���A���̕��@�ł���������Ȃ�
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
        MY_ASSERTION(false, L"���݂��Ȃ��V�[�����I������܂����B");
        break;
    }

    current_scene_type_ = next_scene_;
    next_scene_ = SceneType::NONE;
    if (!current_scene_->Initialize()) {
      return false;
    }
  }

  if (!current_scene_->Update()) {
    MY_LOG(L"�X�V�Ɏ��s���܂���");
    return false;
  }

  return true;
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
