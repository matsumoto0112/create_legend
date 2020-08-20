#ifndef LEGEND_CAMERA_CAMERA_H
#define LEGEND_CAMERA_CAMERA_H

/**
 * @file camera.h
 * @brief カメラクラス定義
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/math/matrix_4x4.h"

namespace legend {
namespace camera {

/**
 * @brief カメラクラス
 */
class Camera {
 public:
  /**
   * @brief コンストラクタ
   */
  Camera();
  /**
   * @brief デストラクタ
   */
  virtual ~Camera();
  /**
   * @brief 初期化
   * @param name カメラ名
   * @param view ビュー行列
   * @param projection プロジェクション行列
   */
  virtual bool Init(const std::wstring& name, const math::Matrix4x4& view,
                    const math::Matrix4x4& projection);
  /**
   * @brief 描画開始
   */
  virtual void RenderStart();
  /**
   * @brief 名前の設定
   */
  inline void SetName(const std::wstring& name) { name_ = name; }
  /**
   * @brief 名前の取得
   */
  inline std::wstring GetName() const { return name_; }

  inline math::Matrix4x4 GetView() const { return view_; }
  inline math::Matrix4x4 GetProjection() const { return projection_; }

 protected:
  /**
   * @brief コンスタントバッファの初期化
   */
  virtual bool InitConstantBuffer();
  /**
   * @brief コンスタントバッファを更新する
   */
  virtual void UpdateConstantBufferStaging();

 protected:
  //! カメラ名
  std::wstring name_;
  //! ビュー行列
  math::Matrix4x4 view_;
  //! プロジェクション行列
  math::Matrix4x4 projection_;

  //! 描画用コンスタントバッファ
  using WorldContext = directx::buffer::constant_buffer_structure::WorldContext;
  using ConstantBuffer = directx::buffer::ConstantBuffer<WorldContext>;
  ConstantBuffer world_context_constant_buffer_;
};

}  // namespace camera
}  // namespace legend
#endif  //! LEGEND_CAMERA_CAMERA_H
