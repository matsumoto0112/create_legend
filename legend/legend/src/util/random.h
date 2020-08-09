#ifndef LEGEND_UTIL_RANDOM_H_
#define LEGEND_UTIL_RANDOM_H_

/**
 * @file random.h
 * @brief ���������f�o�C�X
 */

#include <random>

namespace legend {
namespace util {
/**
 * @class Random
 * @brief ���������N���X
 */
class Random {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Random();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Random();
  /**
   * @brief min�ȏ�max�����̗����̐���
   */
  i32 Range(i32 min, i32 max);
  /**
   * @brief min�ȏ�max�����̗����̐���
   */
  u32 Range(u32 min, u32 max);
  /**
   * @brief min�ȏ�max�ȉ��̗����̐���
   */
  float Range(float min, float max);

 private:
  //! �����f�o�C�X
  std::random_device rnd_;
  //! �^������������
  std::mt19937 mt_;
  //! i32�^�̗�������
  std::uniform_int_distribution<i32> rand_n_;
  //! u32�^�̗�������
  std::uniform_int_distribution<u32> rand_un_;
  //! float�^�̗�������
  std::uniform_real_distribution<float> rand_f_;
};
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RANDOM_H_
