#ifndef LEGEND_SCENES_RESULT_SCENE_H_
#define LEGEND_SCENES_RESULT_SCENE_H_

#include "src/camera/perspective_camera.h"
#include "src/object/back_ground.h"
#include "src/scenes/fade_in_out.h"
#include "src/scenes/scene.h"
#include "src/stage_generate/stage_generator.h"
#include "src/system/game_data.h"
#include "src/ui/ui_board.h"
#include "src/util/timer.h"

namespace legend {
namespace scenes {
using TransformConstantBuffer = directx::buffer::ConstantBuffer<
    directx::buffer::constant_buffer_structure::Transform>;

//���o���
enum class ResultMode {
  WIN_INITIAL,
  CAMERA_CACTH,
  END_PRODUCTION,
  LOSE,
};

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
  /**
   * @brief �X�e�[�W�f�[�^�Ǎ�
   */
  bool LoadStageData(system::GameDataStorage::PlayStageData data,
                     system::GameDataStorage::ResultData result_data);
  /**
   * @brief �������o������
   */
  bool WinProductionInit(
      math::IntVector2 window_size,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id,
      system::GameDataStorage::PlayStageData data);
  /**
   * @brief �s�k���o������
   */
  bool LoseProductionInit(
      math::IntVector2 window_size,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id,
      system::GameDataStorage::PlayStageData data);
  /**
   * @brief �^�[���\������
   */
  bool Turn(math::IntVector2 window_size,
            directx::descriptor_heap::heap_parameter::LocalHeapID heap_id,
            system::GameDataStorage::ResultData data);
  /**
   * @brief �v���C���[�p���[�\������
   */
  bool PlayerPower(
      math::IntVector2 window_size,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id,
      system::GameDataStorage::ResultData data);
  /**
   * @brief �������o�X�V
   */
  void ProductionUpdate();

 private:
  //! UI
  ui::UIBoard board_;
  //! �t�F�[�h
  FadeInOut fade_;
  bool is_scene_end_;
  i32 bgm_;

  //! ���U���g���o���
  ResultMode mode_;
  //! UI�`��J�n�܂ł̎���
  util::CountDownTimer draw_timer_;
  //! �`�悷�邩
  bool is_draw_;

  //! �X�e�[�W�f�[�^
  stage_generate::StageGenerator stage_generator_;
  //! �J����
  camera::PerspectiveCamera camera_;
  //! �J�����������n�߂�܂ł̎���
  util::CountDownTimer start_cacth_timer_;
  //! �w�i
  object::BackGround back_ground_;
  //! ���f��
  std::vector<std::shared_ptr<draw::Model>> models_;
  //! �g�����X�t�H�[��
  std::vector<util::Transform> transforms_;
  //! �R���X�^���g�o�b�t�@
  std::vector<TransformConstantBuffer> transform_cbs_;
};
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_RESULT_SCENE_H_
