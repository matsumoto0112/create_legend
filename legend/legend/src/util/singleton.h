#ifndef LEGEND_UTIL_SINGLETON_H_
#define LEGEND_UTIL_SINGLETON_H_

namespace legend {
namespace util {
/**
 * @class Singleton
 * @brief シングルトン基底クラス
 */
template <class T>
class Singleton {
 public:
  /**
   * @brief インスタンスの取得
   */
  static inline T* GetInstance() {
    struct temp : T {
      temp() : T() {}
    };
    static temp instance;
    return &instance;
  }

 protected:
  /**
   * @brief コンストラクタ
   */
  Singleton(){};
  /**
   * @brief デストラクタ
   */
  virtual ~Singleton() = default;

 private:
  //コピー禁止
  void operator=(const Singleton& other) = delete;
  Singleton(const Singleton& other) = delete;
};

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTL_SINGLETON_H_
