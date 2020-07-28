#ifndef LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_
#define LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_

/**
 * @brief モデル描画テストシーン
 * @details モデルを一つ読み込み、アルベドテクスチャを張った状態で描画するシーン
 パラメータとしてモデルの座標、回転、スケーリング、カメラの座標、回転、fovが操作できる
 */

#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/material/model_material.h"
#include "src/draw/model.h"
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
  //! モデルのトランスフォーム
  util::Transform transform_;
  //! メインカメラ
  camera::PerspectiveCamera camera_;
  //! モデルのマテリアル
  directx::material::ModelMaterial material_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_
