#ifndef LEGEND_SCENES_RESULT_SCENE_H_
#define LEGEND_SCENES_RESULT_SCENE_H_

#include "src/scenes/fade_in_out.h"
#include "src/scenes/scene.h"
#include "src/ui/ui_board.h"

namespace legend {
namespace scenes {

/**
 *@class GameOver
 *@brief �Q�[���I�[�o�[�V�[��
 */
class ResultScene : public Scene {
 public:
  /**
   *@brief �R���X�g���N�^
   */
  ResultScene(ISceneChange* scene_change);
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~ResultScene();
  /**
   *@brief ������
   */
  bool Initialize() override;
  /**
   *@brief �X�V
   */
  bool Update() override;
  /**
   *@brief �`��
   */
  void Draw() override;
  /**
   * @brief �I������
   */
  void Finalize() override;

 private:
  ui::UIBoard board_;

  FadeInOut fade_;
  bool is_scene_end_;
  i32 bgm_;
};
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_RESULT_SCENE_H_
