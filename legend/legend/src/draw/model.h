#ifndef LEGEND_DRAW_MODEL_H_
#define LEGEND_DRAW_MODEL_H_

/**
 * @file model.h
 * @brief モデル管理クラス
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/math/quaternion.h"

namespace legend {
namespace draw {

/**
 * @brief モデル管理
 */
class Model {
 public:
  /**
   * @brief コンストラクタ
   */
  Model();
  /**
   * @brief デストラクタ
   */
  ~Model();
  /**
   * @brief 初期化
   * @param path モデルへのパス
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(const std::filesystem::path& path);
  /**
   * @brief 描画する
   */
  void Draw();

 private:
  //! モデル名
  std::wstring model_name_;
  //! 座標
  math::Vector3 position_;
  //! 回転
  math::Quaternion rotation_;
  //! スケーリング
  math::Vector3 scale_;
  //! 頂点バッファ
  directx::buffer::VertexBuffer vertex_buffer_;
  //! インデックスバッファ
  directx::buffer::IndexBuffer index_buffer_;
  //! アルベドテクスチャ
  directx::buffer::Texture2D albedo_;
  //! トランスフォーム用コンスタントバッファ
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_constant_buffer_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_MODEL_H_
