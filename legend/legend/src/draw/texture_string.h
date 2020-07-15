#ifndef LEGEND_DRAW_TEXTURE_STRING_H_
#define LEGEND_DRAW_TEXTURE_STRING_H_

/**
 * @file texture_string.h
 * @brief 文字列テクスチャ
 */

#include "src/draw/texture_char.h"

namespace legend {
namespace draw {
class TextureString {
  using Chars = std::vector<TextureChar>;

 public:
  TextureString();
  ~TextureString();

  bool Init(const std::wstring& str);
  Chars::iterator begin() { return chars_.begin(); }
  Chars::iterator end() { return chars_.end(); }

 private:
  std::wstring str_;
  Chars chars_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_TEXTURE_STRING_H_
