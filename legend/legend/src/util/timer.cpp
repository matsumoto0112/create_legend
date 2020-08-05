#include "src/util/timer.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {

//コンストラクタ
CountDownTimer::CountDownTimer()
    : current_time_(0.0f), callback_(nullptr), is_pause_(true) {}

//デストラクタ
CountDownTimer::~CountDownTimer() {}

//初期化
void CountDownTimer::Init(float time, CallbackFunc callback, bool start) {
  current_time_ = time;
  callback_ = callback;
  is_pause_ = !start;
}

//更新
bool CountDownTimer::Update() {
  if (is_pause_) {
    return false;
  }

  current_time_ -=
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  if (IsTime()) {
    is_pause_ = true;
    current_time_ = 0.0f;
    if (callback_) callback_();
    return true;
  }

  return false;
}

//時間になったかどうか
bool CountDownTimer::IsTime() const { return current_time_ <= 0.0f; }

//ポーズ
void CountDownTimer::Pause() { is_pause_ = true; }

//再開
void CountDownTimer::Resume() { is_pause_ = false; }

}  // namespace util
}  // namespace legend