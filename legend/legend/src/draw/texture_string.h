#ifndef LEGEND_DRAW_TEXTURE_STRING_H_
#define LEGEND_DRAW_TEXTURE_STRING_H_

/**
 * @file texture_string.h
 * @brief 文字列テクスチャ
 */

#include "src/draw/sprite_renderer.h"
#include "src/draw/texture_char.h"

namespace legend {
namespace draw {
class TextureString {
  using Chars = std::vector<TextureChar>;

 public:
  TextureString();
  ~TextureString();

  bool Init(const std::wstring& str, const std::wstring& font = L"メイリオ",
            i32 font_size = 32);
  void SetPosition(const math::Vector2& position);
  inline math::Vector2 GetPosition() const { return position_; }
  void SetScale(const math::Vector2& scale);
  inline math::Vector2 GetScale() const { return scale_; }
  void Draw(SpriteRenderer& sprite_renderer);
  bool Append(const std::wstring& str);
  void Clear();

 private:
  std::wstring str_;
  Chars chars_;
  math::Vector2 position_;
  math::Vector2 scale_;
  std::wstring font_;
  i32 font_size_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_TEXTURE_STRING_H_
