#ifndef LEGEND_DRAW_TEXTURE_CHAR_H_
#define LEGEND_DRAW_TEXTURE_CHAR_H_

/**
 * @file texture_char.h
 * @brief フォントテクスチャ
 */

#include "src/draw/sprite_2d.h"

namespace legend {
namespace draw {

/**
 * @class TextureChar
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
   * @param command_list コマンドリスト
   * @param c 初期化する文字
   * @param font フォント
   * @param font_size フォントサイズ
   * @param register_num シェーダーのレジスター番号
   * @param handle テクスチャハンドル
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(directx::device::CommandList& command_list, wchar_t c,
            const std::wstring& font, i32 font_size, u32 register_num,
            const directx::descriptor_heap::DescriptorHandle& handle);

 private:
  /**
   * @brief 文字を作成する.
   * @param command_list コマンドリスト
   * @param c 初期化する文字
   * @param font フォント
   * @param font_size フォントサイズ
   * @param data 作成されたデータ
   * @param width 作成されたテクスチャの幅
   * @param height 作成されたテクスチャの高さ
   * @return 作成に成功したらtrueを返す
   */
  bool CreateChar(directx::device::CommandList& command_list, wchar_t c,
                  const std::wstring& font, i32 font_size,
                  std::vector<u8>* data, u32* width, u32* height);
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_TEXTURE_CHAR_H_
