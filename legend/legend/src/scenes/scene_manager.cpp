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
          MY_ASSERTION(false, L"���݂��Ȃ��V�[�����I������܂����B");
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
