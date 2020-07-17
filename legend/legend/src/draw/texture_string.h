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

/**
 * @brief 文字列テクスチャ
 */
class TextureString {
  using Chars = std::vector<TextureChar>;

 public:
  /**
   * @brief コンストラクタ
   */
  TextureString();
  /**
   * @brief デストラクタ
   */
  ~TextureString();

  /**
   * @brief 初期化
   * @param str 初期化する文字列
   * @param font 使用するフォント
   * @param font_size フォントサイズ
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(const std::wstring& str, const std::wstring& font = L"メイリオ",
            i32 font_size = 32);

  /**
   * @brief 座標を設定する
   */
  void SetPosition(const math::Vector2& position);
  /**
   * @brief 座標を取得する
   */
  inline math::Vector2 GetPosition() const { return position_; }
  /**
   * @brief スケーリングを設定する
   */
  void SetScale(const math::Vector2& scale);
  /**
   * @brief スケーリングを取得する
   */
  inline math::Vector2 GetScale() const { return scale_; }
  /**
   * @brief 描画する
   * @param sprite_renderer スプライト描画器
   */
  void Draw(SpriteRenderer& sprite_renderer);
  /**
   * @brief 文字を結合する
   * @param str 結合する文字列
   * @return 結合に成功したらtrueを返す
   */
  bool Append(const std::wstring& str);
  /**
   * @brief 文字列をクリアする
   */
  void Clear();

 private:
  //! 表示する文字列
  std::wstring str_;
  //! 文字列テクスチャ
  Chars chars_;
  //! 座標
  math::Vector2 position_;
  //! スケーリング
  math::Vector2 scale_;
  //! フォント名
  std::wstring font_;
  //! フォントの大きさ
  i32 font_size_;
};

}  // namespace draw
}  // namespace legend

#endif  //! LEGEND_DRAW_TEXTURE_STRING_H_
