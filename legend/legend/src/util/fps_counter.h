#ifndef LEGEND_UTIL_FPS_COUNTER_H_
#define LEGEND_UTIL_FPS_COUNTER_H_

/**
 * @file fps_counter.h
 * @brief フレームレート計測
 */

namespace legend {
namespace util {
class FPSCounter {
  // Tickから秒に変換する係数
  //基本的に1秒は10,000,000Tick
  static constexpr u64 TICKS_PER_SECONDS = 10'000'000;

 public:
  /**
   * @brief コンストラクタ
   */
  FPSCounter();
  /**
   * @brief デストラクタ
   */
  ~FPSCounter();
  /**
   * @brief 更新処理
   */
  void Update();

 public:
  /**
   * @brief アプリケーション開始時から現在までに経過したTick数を取得する
   */
  u64 GetTotalTicks() const { return total_ticks_; }
  /**
   * @brief アプリケーション開始時から現在までに経過した秒数を取得する
   */
  template <typename T = double>
  T GetTotalSeconds() const {
    return static_cast<T>(TicksToSeconds(total_ticks_));
  }
  /**
   * @brief アプリケーション開始時から現在までに経過したフレーム数を取得する
   */
  u64 GetFrameCount() const { return frame_count_; }
  /**
   * @brief 前フレームからの差分Tick数を取得する
   */
  u64 GetDeltaTicks() const { return delta_ticks_; }
  /**
   * @brief 前フレームからの差分時間（秒）を取得する
   */
  template <typename T = double>
  T GetDeltaSeconds() const {
    return static_cast<T>(TicksToSeconds(delta_ticks_));
  }
  /**
   * @brief 前フレームのフレームレートを取得する
   */
  template <typename T = double>
  T GetFPS() const {
    return static_cast<T>(1.0 / GetDeltaSeconds());
  }

 public:
  /**
   * @brief Tick数から秒に変換する
   */
  static double TicksToSeconds(u64 ticks) {
    return static_cast<double>(ticks) / TICKS_PER_SECONDS;
  }
  /**
   * @brief 秒からTick数に変換する
   */
  static u64 SecondsToTicks(double seconds) {
    return static_cast<u64>(seconds * TICKS_PER_SECONDS);
  }

 private:
  //! クロック周波数
  u64 qpc_frequency_;
  //! 前回計測時の時刻
  LARGE_INTEGER qpc_last_time;
  //! 最大差分時間（デバッグ時など、極度に時間がかかった後の計測を平滑化するため）
  u64 max_delta_;
  //! 経過Tick数
  u64 total_ticks_;
  //! 差分Tick数
  u64 delta_ticks_;
  //! フレーム経過数
  u64 frame_count_;
};

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_FPS_COUNTER_H_
