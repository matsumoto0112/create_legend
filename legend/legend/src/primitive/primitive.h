#ifndef LEGEND_PRIMITIVE_PRIMITIVE_H_
#define LEGEND_PRIMITIVE_PRIMITIVE_H_

/**
 * @file primitive.h
 * @brief プリミティブ描画基底クラス定義
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/device/command_list.h"
#include "src/directx/vertex.h"
#include "src/util/transform.h"

namespace legend {
namespace primitive {

/**
 * @brief プリミティブ描画基底クラス
 */
class PrimitiveBase {
 protected:
  using TransformStruct = directx::buffer::constant_buffer_structure::Transform;

 public:
  /**
   * @brief コンストラクタ
   * @param name プリミティブ名
   */
  PrimitiveBase(const std::wstring& name);
  /**
   * @brief デストラクタ
   */
  virtual ~PrimitiveBase();
  /**
   * @brief 初期化
   * @return 初期化に成功したらtrueを返す
   */
  virtual bool Init() = 0;
  /**
   * @brief 描画
   * @param command_list コマンドリスト
   */
  virtual void Render(directx::device::CommandList& command_list);

  /**
   * @brief トランスフォームを設定する
   */
  inline void SetTransform(const util::Transform& transform) {
    this->transform_ = transform;
  }
  /**
   * @brief トランスフォームを取得する
   */
  inline util::Transform GetTransform() const { return this->transform_; }

 protected:
  /**
   * @brief バッファの初期化
   * @param vertices 頂点配列
   * @param indices インデックス配列
   * @return 初期化に成功したらtrueを返す
   */
  virtual bool InitBuffer(const std::vector<directx::PhysicsVertex>& vertices,
                          const std::vector<u16>& indices);

 protected:
  //! プリミティブ名
  std::wstring name_;
  //! 頂点バッファ
  directx::buffer::VertexBuffer vertex_buffer_;
  //! インデックスバッファ
  directx::buffer::IndexBuffer index_buffer_;
  //! トランスフォーム
  util::Transform transform_;
  //! トランスフォームコンスタントバッファ
  directx::buffer::ConstantBuffer<TransformStruct> transform_cb_;
};

}  // namespace primitive
}  // namespace legend
#endif  //! LEGEND_PRIMITIVE_PRIMITIVE_H_
