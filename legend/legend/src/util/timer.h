#ifndef LEGEND_UTIL_TIMER_H_
#define LEGEND_UTIL_TIMER_H_

/**
 * @file timer.h
 * @brief �^�C�}�[�n�N���X��`
 */

namespace legend {
namespace util {

/**
 * @brief �J�E���g�_�E���^�C�}�[
 */
class CountDownTimer final {
 public:
  //! �^�C�}�[�̃R�[���o�b�N�֐�
  using CallbackFunc = std::function<void()>;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  CountDownTimer();
  /**
   * @brief �f�X�g���N�^
   */
  ~CountDownTimer();
  /**
   * @brief ������
   * @param time �J�E���g�_�E�����鎞��
   * @param callback ���ԂɂȂ�����Ă΂��R�[���o�b�N�֐�
   * @param start �^�C�}�[���J�n��Ԃɂ��邩
   */
  void Init(float time, CallbackFunc callback = nullptr, bool start = true);
  /**
   * @brief �X�V����
   * @return ���ԂɂȂ�����true��Ԃ�
   */
  bool Update();
  /**
   * @brief �^�C�}�[��0�ɂȂ������ǂ���
   * @return ���ԂɂȂ��Ă�����true��Ԃ�
   */
  bool IsTime() const;
  /**
   * @brief �^�C�}�[���ꎞ��~����
   */
  void Pause();
  /**
   * @brief �^�C�}�[���ĊJ����
   */
  void Resume();
  /**
   * @brief ���݂̎��Ԃ��擾����
   */
  float CurrentTime() const { return current_time_; }

 private:
  //! ���݂̎���
  float current_time_;
  //! �R�[���o�b�N�֐�
  CallbackFunc callback_;
  //! ���ݒ�~����
  bool is_pause_;
};

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_TIMER_H_
