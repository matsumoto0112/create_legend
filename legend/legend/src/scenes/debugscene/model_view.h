#ifndef LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_
#define LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_

/**
 * @brief モデル描画テストシーン
 * @details モデルを一つ読み込み、アルベドテクスチャを張った状態で描画するシーン
 パラメータとしてモデルの座標、回転、スケーリング、カメラの座標、回転、fovが操作できる
 */

#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/root_signature.h"
#include "src/scenes/scene.h"

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
  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::shader::RootSignature root_signature_;
  ComPtr<ID3D12PipelineState> pipeline_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_MODEL_VIEW_H_
