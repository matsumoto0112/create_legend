#include "src/util/fps_counter.h"

namespace legend {
namespace util {

//コンストラクタ
FPSCounter::FPSCounter() {
  //クロック周波数を取得
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  qpc_frequency_ = freq.QuadPart;
  max_delta_ = qpc_frequency_ / 10;
  //現在時刻を取得
  QueryPerformanceCounter(&qpc_last_time);

  total_ticks_ = 0;
  delta_ticks_ = 0;
  frame_count_ = 0;
}

//デストラクタ
FPSCounter::~FPSCounter() {}

//更新処理
void FPSCounter::Update() {
  //現在時刻を取得
  LARGE_INTEGER current_time;
  QueryPerformanceCounter(&current_time);

  //前Tickとの差文計算
  u64 delta_count = current_time.QuadPart - qpc_last_time.QuadPart;
  qpc_last_time = current_time;

  //最大差分時間を固定化する
  //デバッグ時など、差分時間が一定値を超えるとデバッグにならないので上限設定
  delta_count = std::min(delta_count, max_delta_);

  //差分カウントから差分Tickに変換する
  u64 delta_ticks = delta_count * TICKS_PER_SECONDS / qpc_frequency_;

  this->total_ticks_ += delta_ticks;
  this->frame_count_++;
  this->delta_ticks_ = delta_ticks;
}

}  // namespace util
}  // namespace legend