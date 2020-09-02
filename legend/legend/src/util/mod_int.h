#ifndef LEGEND_UTIL_MOD_INT_H
#define LEGEND_UTIL_MOD_INT_H

/**
 * @file mod_int.h
 * @brief MOD‚ð–@‚Æ‚µ‚½i32Œ^’è‹`
 */
namespace legend {
namespace util {
class ModInt {
 private:
  i32 num;
  i32 mod;

 public:
  ModInt(i32 num = 0, i32 mod = 1) : num((num % mod + mod) % mod), mod(mod){};
  ~ModInt() = default;
  i32 Get() const { return num; }
  ModInt operator++(int) {
    ModInt res(*this);
    this->num = (this->num + 1) % mod;
    return res;
  }
  ModInt& operator++() {
    this->num = (this->num + 1) % mod;
    return *this;
  }
  ModInt& operator--() {
    this->num = (this->num - 1 + mod) % mod;
    return *this;
  }
  ModInt operator--(int) {
    ModInt res(*this);
    this->num = (this->num - 1 + mod) % mod;
    return res;
  }
};

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_MOD_INT_H
