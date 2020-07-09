#include "src/util/fps_counter.h"

namespace legend {
namespace util {

//�R���X�g���N�^
FPSCounter::FPSCounter() {
  //�N���b�N���g�����擾
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  qpc_frequency_ = freq.QuadPart;
  max_delta_ = qpc_frequency_ / 10;
  //���ݎ������擾
  QueryPerformanceCounter(&qpc_last_time);

  total_ticks_ = 0;
  delta_ticks_ = 0;
  frame_count_ = 0;
}

//�f�X�g���N�^
FPSCounter::~FPSCounter() {}

//�X�V����
void FPSCounter::Update() {
  //���ݎ������擾
  LARGE_INTEGER current_time;
  QueryPerformanceCounter(&current_time);

  //�OTick�Ƃ̍����v�Z
  u64 delta_count = current_time.QuadPart - qpc_last_time.QuadPart;
  qpc_last_time = current_time;

  //�ő卷�����Ԃ��Œ艻����
  //�f�o�b�O���ȂǁA�������Ԃ����l�𒴂���ƃf�o�b�O�ɂȂ�Ȃ��̂ŏ���ݒ�
  delta_count = std::min(delta_count, max_delta_);

  //�����J�E���g���獷��Tick�ɕϊ�����
  u64 delta_ticks = delta_count * TICKS_PER_SECONDS / qpc_frequency_;

  this->total_ticks_ += delta_ticks;
  this->frame_count_++;
  this->delta_ticks_ = delta_ticks;
}

}  // namespace util
}  // namespace legend