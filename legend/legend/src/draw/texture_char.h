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

/**
 * @brief 文字テクスチャクラス
 */
class TextureChar : public Sprite2D {
 public:
  /**
   * @brief コンストラクタ
   */
  TextureChar();
  /**
   * @brief デストラクタ
   */
  ~TextureChar();
  /**
   * @brief 初期化
   * @param c 初期化する文字
   * @param font フォント
   * @param font_size フォントサイズ
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(wchar_t c, const std::wstring& font, i32 font_size);

 private:
  /**
   * @brief 文字を作成する
   * @param c 初期化する文字
   * @param font フォント
   * @param font_size フォントサイズ
   * @param data 作成されたデータ
   * @param width 作成されたテクスチャの幅
   * @param height 作成されたテクスチャの高さ
   * @return 作成に成功したらtrueを返す
   */
  bool CreateChar(wchar_t c, const std::wstring& font, i32 font_size,
                  std::vector<u8>* data, u32* width, u32* height);
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_TEXTURE_CHAR_H_
