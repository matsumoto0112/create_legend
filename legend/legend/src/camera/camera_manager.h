#ifndef LEGEND_CAMERA_CAMERA_MANAGER_H
#define LEGEND_CAMERA_CAMERA_MANAGER_H

/**
 * @file camera_manager.h
 * @brief �J�����Ǘ��N���X��`
 */

#include "src/actor/actor_mediator.h"
#include "src/camera/lookat_follow_target_camera.h"
#include "src/camera/lookat_target_camera.h"
#include "src/camera/perspective_camera.h"
#include "src/stage_generate/camera_generate_info.h"

namespace legend {
namespace camera {

/**
 * @brief �J�����̃��[�h��`
 */
namespace camera_mode {
enum Enum { PLAYER_LOOKAT, BIRDS_EYE_VIEW, FREE };
}  // namespace camera_mode

/**
 * @brief �J�����Ǘ��N���X
 */
class CameraManager {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  CameraManager();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~CameraManager();
  /**
   * @brief ������
   * @param mediator �A�N�^�[����I�u�W�F�N�g
   * @param infos �t���[�J�����̐�����񃊃X�g
   */
  bool Init(actor::IActorMediator* mediator,
            const std::vector<stage_generate::CameraGenerateInfo>& infos);
  /**
   * @brief �J�����̍X�V����
   */
  void UpdateCamera();
  /**
   * @brief �J�����̍X�V��L���ɂ��邩�ǂ����̃t���O���Z�b�g����
   */
  void SetUpdateEnable(bool enable);
  /**
   * @brief �J�����̃��[�h��ݒ肷��
   */
  void SetCameraMode(camera_mode::Enum camera_mode);
  /**
   * @brief �J�����̃��[�h���擾����
   */
  camera_mode::Enum GetCameraMode() const { return current_camera_mode_; }
  /**
   * @brief ���݂̃��[�h�ɉ������J�������擾����
   */
  camera::Camera* GetCurrentCamera() const;
  /**
   * @brief �v���C���[�𒍎�����J�������擾����
   */
  camera::LookatFollowTargetCamera* GetPlayerLookatCamera() const {
    return player_lookat_camera_.get();
  }
  /**
   * @brief �`��J�n
   */
  void RenderStart();

 private:
  //! �J�����̍X�V��L���ɂ��邩
  bool update_enable_;
  //! �A�N�^�[����I�u�W�F�N�g
  actor::IActorMediator* actor_mediator_;
  //! ���݂̃J�����̃��[�h
  camera_mode::Enum current_camera_mode_;
  //! �v���C���[�𒍎�����J����
  std::unique_ptr<camera::LookatFollowTargetCamera> player_lookat_camera_;
  //! ���Վ��_�̃J����
  std::unique_ptr<camera::PerspectiveCamera> birds_eye_view_camera_;
  //! �t���[�J����
  std::vector<std::unique_ptr<LookatTargetCamera>> free_cameras_;
};

}  // namespace camera
}  // namespace legend
#endif  //! LEGEND_CAMERA_CAMERA_MANAGER_H