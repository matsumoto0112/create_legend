#ifndef LEGEND_OBJECT_DESK_H_
#define LEGEND_OBJECT_DESK_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"

/**
 * @file desk.h
 * @brief 机クラス
 */

namespace legend {
namespace object {

/**
 * @brief 机
 */
class Desk : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief 机の初期化パラメータ
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bounding_box_length;
    math::Vector3 normal;
    float mass = 0.0f;
    float restitution = 0.5f;
    float friction = 1.5f;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  Desk();
  /**
   * @brief デストラクタ
   */
  virtual ~Desk();

  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter);
  /**
   * @brief 更新
   */
  bool Update() override;
  virtual void Destroy() override;
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
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace object
}  // namespace legend

#endif  //! LEGEND_OBJECT_DESK_H_
