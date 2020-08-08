#ifndef LEGEND_UTIL_RANDOM_H_
#define LEGEND_UTIL_RANDOM_H_

/**
 * @file random.h
 * @brief 乱数生成デバイス
 */

#include <random>

namespace legend {
namespace util {
/**
 * @class Random
 * @brief 乱数生成クラス
 */
class Random {
 public:
  /**
   * @brief コンストラクタ
   */
  Random();
  /**
   * @brief デストラクタ
   */
  virtual ~Random();
  /**
   * @brief min以上max未満の乱数の生成
   */
  i32 Range(i32 min, i32 max);
  /**
   * @brief min以上max未満の乱数の生成
   */
  u32 Range(u32 min, u32 max);
  /**
   * @brief min以上max以下の乱数の生成
   */
  float Range(float min, float max);

 private:
  //! 乱数デバイス
  std::random_device rnd_;
  //! 疑似乱数生成器
  std::mt19937 mt_;
  //! i32型の乱数生成
  std::uniform_int_distribution<i32> rand_n_;
  //! u32型の乱数生成
  std::uniform_int_distribution<u32> rand_un_;
  //! float型の乱数生成
  std::uniform_real_distribution<float> rand_f_;
};
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RANDOM_H_
