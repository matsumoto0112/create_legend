#ifndef LEGEND_CAMERA_CAMERA_MANAGER_H
#define LEGEND_CAMERA_CAMERA_MANAGER_H

/**
 * @file camera_manager.h
 * @brief カメラ管理クラス定義
 */

#include "src/actor/actor_mediator.h"
#include "src/camera/lookat_follow_target_camera.h"
#include "src/camera/lookat_target_camera.h"
#include "src/camera/perspective_camera.h"
#include "src/stage_generate/camera_generate_info.h"

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
   * @param infos フリーカメラの生成情報リスト
   */
  bool Init(actor::IActorMediator* mediator,
            const std::vector<stage_generate::CameraGenerateInfo>& infos);
  /**
   * @brief カメラの更新処理
   */
  void UpdateCamera();
  /**
   * @brief カメラの更新を有効にするかどうかのフラグをセットする
   */
  void SetUpdateEnable(bool enable);
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
  camera::Camera* GetCurrentCamera();
  /**
   * @brief プレイヤーを注視するカメラを取得する
   */
  camera::LookatFollowTargetCamera* GetPlayerLookatCamera() const {
    return player_lookat_camera_.get();
  }
  /**
   * @brief 描画開始
   */
  void RenderStart();

 private:
  //! カメラの更新を有効にするか
  bool update_enable_;
  //! アクター仲介オブジェクト
  actor::IActorMediator* actor_mediator_;
  //! 現在のカメラのモード
  camera_mode::Enum current_camera_mode_;
  //! プレイヤーを注視するカメラ
  std::unique_ptr<camera::LookatFollowTargetCamera> player_lookat_camera_;
  //! 俯瞰視点のカメラ
  std::unique_ptr<camera::PerspectiveCamera> birds_eye_view_camera_;
  //! フリーカメラ
  std::vector<std::unique_ptr<LookatTargetCamera>> free_cameras_;

  //! 描画カメラを指すID
  int id;
  //! カメラの種類
  enum Type {
    Type_A,  //ランダム
    Type_B,  //遠いやつ
    Type_C,  //近いやつ
    MAX
  } type_;
};

}  // namespace camera
}  // namespace legend
#endif  //! LEGEND_CAMERA_CAMERA_MANAGER_H
