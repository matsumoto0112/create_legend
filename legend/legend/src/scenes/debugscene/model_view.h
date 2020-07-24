#ifndef LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_
#define LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_

/**
 * @brief モデル描画テストシーン
 * @details モデルを一つ読み込み、アルベドテクスチャを張った状態で描画するシーン
 パラメータとしてモデルの座標、回転、スケーリング、カメラの座標、回転、fovが操作できる
 */

#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/render_target/multi_render_target_texture.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/root_signature.h"
#include "src/draw/model.h"
#include "src/math/matrix_4x4.h"
#include "src/math/quaternion.h"
#include "src/scenes/scene.h"
#include "src/util/transform.h"

namespace legend {
namespace scenes {
namespace debugscene {

/**
 * @class ModelView
 * @brief モデル描画テストシーン
 */
class ModelView : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  ModelView(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  ~ModelView();
  /**
   * @brief 初期化
   */
  bool Initialize() override;
  /**
   * @brief 更新
   */
  bool Update() override;
  /**
   * @brief 描画
   */
  void Draw() override;

  /**
   * @brief 終了処理
   */
  void Finalize() override;

 private:
  //! モデル名
  static const std::wstring MODEL_NAME;
  //! 読み込んだモデルデータ
  draw::Model model_;
  //! トランスフォーム転送用コンスタントバッファ
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_cb_;
  util::Transform transform_;
  //描画マテリアル
  directx::shader::GraphicsPipelineState pipeline_state_;

  camera::PerspectiveCamera camera_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_
