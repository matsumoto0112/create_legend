#ifndef LEGEND_PLAYER_PLAYER_H_
#define LEGEND_PLAYER_PLAYER_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/skill/skill_manager.h"

namespace legend {
namespace player {

/**
 * @class Player
 * @brief プレイヤーのクラス
 */
class Player : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief 初期化パラメータ
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bouding_box_length;
    float min_power;
    float max_power;
    float min_strength;
    float max_strength;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  Player();
  /**
   * @brief デストラクタ
   */
  ~Player();
  /**
   * @brief 初期化
   */
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter);
  /**
   * @brief 更新
   */
  bool Update();
  /**
   * @brief 描画
   */
  void Draw() override;
  /**
   * @brief 座標の設定
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief 回転量の設定
   */
  void SetRotation(math::Quaternion rotation);
  /**
   * @brief スケールの設定
   */
  void SetScale(math::Vector3 scale);
  /**
   * @brief 移動量の設定
   */
  void CheckImpulse();
  /**
   * @brief 移動量の設定
   */
  void SetVelocity(math::Vector3 velocity);
  /**
   * @brief 加える力の設定
   */
  void SetImpulse();
  /**
   * @brief 移動に必要なパラメータの初期化
   */
  void ResetParameter();
  /**
   * @brief 座標の取得
   */
  math::Vector3 GetPosition() const;
  /**
   * @brief スケールの取得
   */
  math::Vector3 GetScale() const;
  /**
   * @brief 移動量の取得
   */
  math::Vector3 GetVelocity() const;
  /**
   * @brief 回転の取得
   */
  math::Quaternion GetRotation() const;
  /**
   * @brief 加える力の割合の取得
   */
  float GetImpulse() const;
  /**
   * @brief 加える力の取得
   */
  float GetPower() const;
  /**
   * @brief 行動終了の取得
   */
  bool GetMoveEnd() const;
  /**
   * @brief 移動判定の取得
   */
  bool GetIsMove() const;
  /**
   * @brief 強化パラメータの更新
   */
  void UpdateStrength(const float& add_strength);
  /**
   * @brief 現在の強化度合いを取得
   */
  float GetStrength() const;
  /**
   * @brief スキル選択中かを取得
   */
  bool GetSkillSelect();
  /**
   * @brief スキル選択中かを取得
   */
  bool GetPlayerDeathFlag();
  /**
   * @brief 別のコライダーと衝突したときのコールバック
   */
  void OnHit(bullet::Collider* other);

  bullet::Collider* GetCollider() const { return box_.get(); }

  void SkillUpdate();

 private:
  //! 速度
  math::Vector3 velocity_;
  math::Vector3 input_velocity_;
  math::Vector3 change_amount_velocity_;
  //! 移動中か
  bool is_move_;

  //速度の更新時間
  float velocity_update_time_;
  const float change_time_ = 0.1f;

  //! 移動に加える力
  const float power_ = 50.0f;
  //! 実際に加える力の加減
  float impulse_;
  //! 最小値
  float min_power_;
  //! 最大値
  float max_power_;
  //! 入力状態か
  bool is_input_;

  //! ゲージが上昇かどうか
  bool up_power_;
  //! パワー設定終了か
  bool is_set_power_;

  //! 更新時間
  float update_time_;

  //! 強化パラメータ
  float strength_;
  //! 強化上限
  float max_strength_;
  //! 強化下限
  float min_strength_;

  //! スキルマネージャー
  skill::SkillManager skill_manager_;

  //! コライダー
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace player
}  // namespace legend

#endif  //! LEGEND_PLAYER_PLAYER_H_
