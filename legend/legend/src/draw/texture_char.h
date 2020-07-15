#ifndef LEGEND_DRAW_TEXTURE_CHAR_H_
#define LEGEND_DRAW_TEXTURE_CHAR_H_

/**
 * @file texture_char.h
 * @brief フォントテクスチャ
 */

#include "src/directx/buffer/texture_2d.h"
#include "src/draw/sprite_2d.h"

namespace legend {
namespace draw {
class TextureChar : public Sprite2D {
 public:
  TextureChar();
  ~TextureChar();
  bool Init(wchar_t c);

 private:
  Sprite2D sprite_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_TEXTURE_CHAR_H_
