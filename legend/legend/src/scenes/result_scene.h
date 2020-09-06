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

//演出状態
enum class ResultMode {
  WIN_INITIAL,
  CAMERA_CACTH,
  END_PRODUCTION,
  LOSE,
};

/**
 *@class GameOver
 *@brief ゲームオーバーシーン
 */
class ResultScene : public Scene {
 public:
  /**
   *@brief コンストラクタ
   */
  ResultScene(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  virtual ~ResultScene();
  /**
   *@brief 初期化
   */
  bool Initialize() override;
  /**
   *@brief 更新
   */
  bool Update() override;
  /**
   *@brief 描画
   */
  void Draw() override;
  /**
   * @brief 終了処理
   */
  void Finalize() override;
  /**
   * @brief ステージデータ読込
   */
  bool LoadStageData(system::GameDataStorage::PlayStageData data,
                     system::GameDataStorage::ResultData result_data);
  /**
   * @brief 勝利演出初期化
   */
  bool WinProductionInit(
      math::IntVector2 window_size,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id,
      system::GameDataStorage::PlayStageData data);
  /**
   * @brief 敗北演出初期化
   */
  bool LoseProductionInit(
      math::IntVector2 window_size,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id,
      system::GameDataStorage::PlayStageData data);
  /**
   * @brief ターン表示処理
   */
  bool Turn(math::IntVector2 window_size,
            directx::descriptor_heap::heap_parameter::LocalHeapID heap_id,
            system::GameDataStorage::ResultData data);
  /**
   * @brief プレイヤーパワー表示処理
   */
  bool PlayerPower(
      math::IntVector2 window_size,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id,
      system::GameDataStorage::ResultData data);
  /**
   * @brief 勝利演出更新
   */
  void ProductionUpdate();

 private:
  //! UI
  ui::UIBoard board_;
  //! フェード
  FadeInOut fade_;
  bool is_scene_end_;
  i32 bgm_;

  //! リザルト演出状態
  ResultMode mode_;
  //! UI描画開始までの時間
  util::CountDownTimer draw_timer_;
  //! 描画するか
  bool is_draw_;

  //! ステージデータ
  stage_generate::StageGenerator stage_generator_;
  //! カメラ
  camera::PerspectiveCamera camera_;
  //! カメラを引き始めるまでの時間
  util::CountDownTimer start_cacth_timer_;
  //! 背景
  object::BackGround back_ground_;
  //! モデル
  std::vector<std::shared_ptr<draw::Model>> models_;
  //! トランスフォーム
  std::vector<util::Transform> transforms_;
  //! コンスタントバッファ
  std::vector<TransformConstantBuffer> transform_cbs_;
};
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_RESULT_SCENE_H_
