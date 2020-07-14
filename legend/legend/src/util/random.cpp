#include "src/util/random.h"

namespace legend {
namespace util {

//コンストラクタ
Random::Random() { mt_ = std::mt19937(rnd_()); }

//デストラクタ
Random::~Random() {}

// min以上max未満の乱数生成
i32 Random::Range(i32 min, i32 max) {
  rand_n_ = std::uniform_int_distribution<i32>(min, max - 1);
  return rand_n_(mt_);
}

// min以上max以下の乱数生成
float Random::Range(float min, float max) {
  rand_f_ = std::uniform_real_distribution<float>(min, max);
  return rand_f_(mt_);
}

}  // namespace util
}  // namespace legend
