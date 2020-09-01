#ifndef LEGEND_CAMERA_CAMERA_MANAGER_H
#define LEGEND_CAMERA_CAMERA_MANAGER_H

/**
 * @file camera_manager.h
 * @brief カメラ管理クラス定義
 */

#include "src/actor/actor_mediator.h"
#include "src/camera/lookat_target_camera.h"
#include "src/camera/perspective_camera.h"

namespace legend {
namespace camera {

/**
 * @brief カメラのモード定義
 */
namespace camera_mode {
enum Enum { PLAYER_LOOKAT, BIRDS_EYE_VIEW, FREE };
}  // namespace camera_mode

/**
 * @brief カメラ管理クラス
 */
class CameraManager {
 public:
  /**
   * @brief コンストラクタ
   */
  CameraManager();
  /**
   * @brief デストラクタ
   */
  virtual ~CameraManager();
  /**
   * @brief 初期化
   * @param mediator アクター仲介オブジェクト
   */
  bool Init(actor::IActorMediator* mediator);
  /**
   * @brief カメラの更新処理
   */
  void UpdateCamera();
  /**
   * @brief カメラのモードを設定する
   */
  void SetCameraMode(camera_mode::Enum camera_mode);
  /**
   * @brief カメラのモードを取得する
   */
  camera_mode::Enum GetCameraMode() const { return current_camera_mode_; }
  /**
   * @brief 現在のモードに応じたカメラを取得する
   */
  camera::Camera* GetCurrentCamera() const;
  /**
   * @brief プレイヤーを注視するカメラを取得する
   */
  camera::LookAtTargetCamera* GetPlayerLookatCamera() const {
    return player_lookat_camera_.get();
  }
  /**
   * @brief 描画開始
   */
  void RenderStart();

 private:
  //! アクター仲介オブジェクト
  actor::IActorMediator* actor_mediator_;
  //! 現在のカメラのモード
  camera_mode::Enum current_camera_mode_;
  //! プレイヤーを注視するカメラ
  std::unique_ptr<camera::LookAtTargetCamera> player_lookat_camera_;
  //! 俯瞰視点のカメラ
  std::unique_ptr<camera::PerspectiveCamera> birds_eye_view_camera_;
};

}  // namespace camera
}  // namespace legend
#endif  //! LEGEND_CAMERA_CAMERA_MANAGER_H
