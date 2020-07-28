#ifndef LEGEND_DIRECTX_MATERIAL_MODEL_MATERIAL_H_
#define LEGEND_DIRECTX_MATERIAL_MODEL_MATERIAL_H_

/**
 * @file model_material.h
 * @brief model_view.hlsl使用の通常モデル描画用マテリアル定義
 */

#include "src/directx/material/material.h"

namespace legend {
namespace directx {
namespace material {

/**
 * @brief model_view.hlsl使用の通常モデル描画用マテリアル
 */
class ModelMaterial : public IMaterial {
 public:
  /**
   * @brief コンストラクタ
   */
  ModelMaterial();
  /**
   * @brief デストラクタ
   */
  ~ModelMaterial();
  /**
   * @brief 初期化
   * @return 初期化に成功したらtrueを返す
   */
  virtual bool Init() override;
  /**
   * @brief グラフィックコマンドリストにセットする
   */
  virtual void SetToGraphicsCommandList() override;
};

}  // namespace material
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_MATERIAL_MODEL_MATERIAL_H_
