#ifndef LEGEND_OBJECT_DESK_H_
#define LEGEND_OBJECT_DESK_H_

#include "src/actor/actor.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/draw/model.h"
#include "src/physics/bounding_box.h"
#include "src/util/transform.h"

namespace legend {
namespace object {

class Desk : public actor::Actor<physics::BoundingBox> {
 public:
  /**
   * @brief 机の初期化パラメータ
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bounding_box_length;
    math::Vector3 normal;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  Desk();
  /**
   * @brief デストラクタ
   */
  ~Desk();

  virtual bool Init(const InitializeParameter& parameter);
  /**
   * @brief 更新
   */
  bool Update() override;
  /**
   * @brief 座標の設定
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief 回転の設定
   */
  void SetRotation(math::Quaternion rotation);
  /**
   * @brief スケールの設定
   */
  void SetScale(math::Vector3 scale);
  /**
   * @brief 法線ベクトルの設定
   */
  void SetNormal(math::Vector3 normal);
  /**
   * @brief 座標の取得
   */
  math::Vector3 GetPosition();
  /**
   * @brief 回転の取得
   */
  math::Quaternion GetRotation();
  /**
   * @brief スケールの設定
   */
  math::Vector3 GetScale();
  /**
   * @brief 法線ベクトルの取得
   */
  math::Vector3 GetNormal();

 private:
  math::Vector3 normal_;
};

}  // namespace object
}  // namespace legend

#endif  //! LEGEND_OBJECT_DESK_H_
