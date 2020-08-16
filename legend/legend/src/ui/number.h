#ifndef LEGEND_UI_NUMBER_H
#define LEGEND_UI_NUMBER_H

/**
 * @file number.h
 * @brief 1桁の数値を描画するUI画像クラス定義
 */

#include "src/ui/image.h"

namespace legend {
namespace ui {

/**
 * @brief 1桁の数値を描画するUI画像クラス
 */
class Number : public Image {
 public:
  /**
   * @brief コンストラクタ
   */
  Number();
  /**
   * @brief デストラクタ
   */
  virtual ~Number();
  /**
   * @brief 初期化
   * @param name 画像名
   * @param heap_id 使用するヒープID
   */
  virtual bool Init(
      const std::wstring& name,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id =
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)
      override;
  /**
   * @brief 数値の設定
   * @param 0~9までの数値
   */
  void SetNumber(int num);
};

}  // namespace ui
}  // namespace legend
#endif  //! LEGEND_UI_NUMBER_H
