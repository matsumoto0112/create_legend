#ifndef LEGEND_UTIL_TIMER_H_
#define LEGEND_UTIL_TIMER_H_

/**
 * @file timer.h
 * @brief タイマー系クラス定義
 */

namespace legend {
namespace util {

/**
 * @brief カウントダウンタイマー
 */
class CountDownTimer final {
 public:
  //! タイマーのコールバック関数
  using CallbackFunc = std::function<void()>;

 public:
  /**
   * @brief コンストラクタ
   */
  CountDownTimer();
  /**
   * @brief デストラクタ
   */
  ~CountDownTimer();
  /**
   * @brief 初期化
   * @param time カウントダウンする時間
   * @param callback 時間になったら呼ばれるコールバック関数
   * @param start タイマーを開始状態にするか
   */
  void Init(float time, CallbackFunc callback = nullptr, bool start = true);
  /**
   * @brief 更新処理
   * @return 時間になったらtrueを返す
   */
  bool Update();
  /**
   * @brief タイマーが0になったかどうか
   * @return 時間になっていたらtrueを返す
   */
  bool IsTime() const;
  /**
   * @brief タイマーを一時停止する
   */
  void Pause();
  /**
   * @brief タイマーを再開する
   */
  void Resume();
  /**
   * @brief 現在の時間を取得する
   */
  float CurrentTime() const { return current_time_; }

 private:
  //! 現在の時間
  float current_time_;
  //! コールバック関数
  CallbackFunc callback_;
  //! 現在停止中か
  bool is_pause_;
};

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_TIMER_H_
