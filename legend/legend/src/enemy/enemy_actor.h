#ifndef LEGEND_ENEMY_ENEMY_ACTTOR_H_
#define LEGEND_ENEMY_ENEMY_ACTTOR_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/draw/particle/particle_emitter.h"
#include "src/enemy/enemy_ai.h"
#include "src/enemy/enemy_type.h"
#include "src/util/transform.h"

namespace legend {
namespace enemy {

/**
 * @class Enemy
 * @brief エネミーのクラス
 */
class EnemyActor : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief 初期化パラメータ
   */
  struct InitializeParameter {
    //! トランスフォーム
    util::Transform transform;
    //! 衝突判定
    math::Vector3 bouding_box_length;
    //! 重さ
    float mass = 1.5f;
    //! 反発
    float restitution = 0.5f;
    //! 摩擦
    float friction = 1.0f;
    //! モデルID
    i32 model_id = 0;
    //! 種類番号
    i32 type_index = 0;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  EnemyActor();
  /**
   * @brief デストラクタ
   */
  ~EnemyActor();
  /**
   * @brief 初期化
   * @param mediator 仲介者
   * @param parameter 初期化パラメータ
   * @return 初期化成功時TRUE
   */
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter);
  /**
   * @brief 削除
   */
  virtual void Remove();
  /**
   * @brief 更新
   */
  virtual bool Update();
  /**
   * @brief 描画
   */
  virtual void Draw();

  /**
   * @brief 座標の設定
   * @param position 座標
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief 速度の設定
   * @param velocity 速度
   */
  void SetVelocity(math::Vector3 velocity);
  /**
   * @brief 回転量の設定
   */
  void SetRotation();
  /**
   * @brief 移動に必要なパラメータの初期化
   */
  void ResetParameter();
  /**
   * @brief 座標の取得
   * @return 座標取得
   */
  math::Vector3 GetPosition() const { return transform_.GetPosition(); }
  /**
   * @brief 移動量の取得
   * @return 速度取得
   */
  virtual math::Vector3 GetVelocity() const { return (box_->GetVelocity()); }
  /**
   * @brief 回転の取得
   * @return 回転取得
   */
  math::Quaternion GetRotation() const { return transform_.GetRotation(); }

  /**
   * @brief 移動に加える力の取得
   * @return パワー取得
   */
  float GetPower() const { return power_; }
  /**
   * @brief 移動に加える力の取得
   * @return 強化値取得
   */
  float GetStrength() const { return strength_; }
  /**
   * @brief 移動終了判定の取得
   * @return 移動終了しているか判定
   */
  bool GetMoveEnd() const { return (!is_move_ && move_end_); }
  /**
   * @brief 移動終了判定のリセット
   */
  void ResetMoveEnd() { move_end_ = false; }
  /**
   * @brief プレイヤーとの距離
   * @return 距離の取得
   */
  float DistanceWithPlayer();
  /**
   * @brief 移動タイプ取得
   * @return 移動タイプ取得
   */
  enemy::enemy_type::MoveType GetMoveType() { return enemy_ai_.move_type_; };
  /**
   * @brief 衝突取得
   * @return 衝突取得
   */
  bullet::BoundingBox* GetCollider() const { return box_.get(); }
  /**
   * @brief エネミータイプ設定
   * @param type_index 敵タイプ番号
   */
  virtual void SetType(i32 type_index = 0);
  /**
   * @brief 衝突判定
   * @param 衝突相手
   */
  virtual void OnHit(bullet::Collider* other);
  /**
   * @brief 衝突時行動
   * @param 衝突相手
   */
  void HitAction(bullet::Collider* other);
  /**
   * @brief 強化パラメータの更新
   * @param 変更値
   */
  void UpdateStrength(const float& weak);
  /**
   * @brief パーティクル生成
   * @param トランスフォーム
   */
  void CreateFireParticle(const util::Transform& transform);

 protected:
  //! 移動中か
  bool is_move_;
  //! 一度のアクションでの衝突判定
  bool is_hit_done_;

  //! 移動に加える力
  const float power_ = 1;

  //! 更新時間
  float update_time_;

  //! 移動終了判定
  bool move_end_;
  //! 強化値
  float strength_;
  //! 強化値最低値
  float min_strength_;

  //! エネミーAI
  enemy::EnemyAI enemy_ai_;

  //! 衝突判定
  std::shared_ptr<bullet::BoundingBox> box_;
  //! パーティクル
  std::shared_ptr<draw::particle::ParticleEmitter> enemy_move_particle_;

  //! 障害物に衝突したか
  bool is_hit_obstacle_;
  //! カウントダウンタイマー
  util::CountDownTimer obstacle_hit_timer_;
};

}  // namespace enemy
}  // namespace legend

#endif  //! LEGEND_ENEMY_ENEMY_ACTTOR_H_