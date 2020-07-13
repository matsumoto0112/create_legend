#include "src/util/random.h"

namespace legend {
namespace util {

//�R���X�g���N�^
Random::Random() { mt_ = std::mt19937(rnd_()); }

//�f�X�g���N�^
Random::~Random() {}

// min�ȏ�max�����̗�������
i32 Random::Range(i32 min, i32 max) {
  rand_n_ = std::uniform_int_distribution<i32>(min, max - 1);
  return rand_n_(mt_);
}

// min�ȏ�max�ȉ��̗�������
float Random::Range(float min, float max) {
  rand_f_ = std::uniform_real_distribution<float>(min, max);
  return rand_f_(mt_);
}

}  // namespace util
}  // namespace legend
