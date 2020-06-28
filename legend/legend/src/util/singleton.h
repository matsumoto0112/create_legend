#ifndef LEGEND_UTIL_SINGLETON_H_
#define LEGEND_UTIL_SINGLETON_H_

namespace legend {
namespace util {
/**
 * @class Singleton
 * @brief �V���O���g�����N���X
 */
template <class T>
class Singleton {
 public:
  /**
   * @brief �C���X�^���X�̎擾
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
   * @brief �R���X�g���N�^
   */
  Singleton(){};
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Singleton() = default;

 private:
  //�R�s�[�֎~
  void operator=(const Singleton& other) = delete;
  Singleton(const Singleton& other) = delete;
};

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTL_SINGLETON_H_
