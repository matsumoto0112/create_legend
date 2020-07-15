#ifndef LEGEND_DRAW_TEXTURE_CHAR_H_
#define LEGEND_DRAW_TEXTURE_CHAR_H_

/**
 * @file texture_char.h
 * @brief �t�H���g�e�N�X�`��
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
  bool ChangeChar(wchar_t c);

 private:
  bool CreateChar(wchar_t c, std::vector<u8>* data, u32* width, u32* height);
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_TEXTURE_CHAR_H_
