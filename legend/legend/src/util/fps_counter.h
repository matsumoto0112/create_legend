#ifndef LEGEND_UTIL_FPS_COUNTER_H_
#define LEGEND_UTIL_FPS_COUNTER_H_

/**
 * @file fps_counter.h
 * @brief �t���[�����[�g�v��
 */

namespace legend {
namespace util {
class FPSCounter {
  // Tick����b�ɕϊ�����W��
  //��{�I��1�b��10,000,000Tick
  static constexpr u64 TICKS_PER_SECONDS = 10'000'000;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  FPSCounter();
  /**
   * @brief �f�X�g���N�^
   */
  ~FPSCounter();
  /**
   * @brief �X�V����
   */
  void Update();

 public:
  /**
   * @brief �A�v���P�[�V�����J�n�����猻�݂܂łɌo�߂���Tick�����擾����
   */
  u64 GetTotalTicks() const { return total_ticks_; }
  /**
   * @brief �A�v���P�[�V�����J�n�����猻�݂܂łɌo�߂����b�����擾����
   */
  template <typename T = double>
  T GetTotalSeconds() const {
    return static_cast<T>(TicksToSeconds(total_ticks_));
  }
  /**
   * @brief �A�v���P�[�V�����J�n�����猻�݂܂łɌo�߂����t���[�������擾����
   */
  u64 GetFrameCount() const { return frame_count_; }
  /**
   * @brief �O�t���[������̍���Tick�����擾����
   */
  u64 GetDeltaTicks() const { return delta_ticks_; }
  /**
   * @brief �O�t���[������̍������ԁi�b�j���擾����
   */
  template <typename T = double>
  T GetDeltaSeconds() const {
    return static_cast<T>(TicksToSeconds(delta_ticks_));
  }
  /**
   * @brief �O�t���[���̃t���[�����[�g���擾����
   */
  template <typename T = double>
  T GetFPS() const {
    return static_cast<T>(1.0 / GetDeltaSeconds());
  }

 public:
  /**
   * @brief Tick������b�ɕϊ�����
   */
  static double TicksToSeconds(u64 ticks) {
    return static_cast<double>(ticks) / TICKS_PER_SECONDS;
  }
  /**
   * @brief �b����Tick���ɕϊ�����
   */
  static u64 SecondsToTicks(double seconds) {
    return static_cast<u64>(seconds * TICKS_PER_SECONDS);
  }

 private:
  //! �N���b�N���g��
  u64 qpc_frequency_;
  //! �O��v�����̎���
  LARGE_INTEGER qpc_last_time;
  //! �ő卷�����ԁi�f�o�b�O���ȂǁA�ɓx�Ɏ��Ԃ�����������̌v���𕽊������邽�߁j
  u64 max_delta_;
  //! �o��Tick��
  u64 total_ticks_;
  //! ����Tick��
  u64 delta_ticks_;
  //! �t���[���o�ߐ�
  u64 frame_count_;
};

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_FPS_COUNTER_H_
