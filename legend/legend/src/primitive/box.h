#ifndef LEGEND_PRIMITIVE_BOX_H_
#define LEGEND_PRIMITIVE_BOX_H_

/**
 * @file box.h
 * @brief Box型のプリミティブ描画クラス定義
 */

#include "src/primitive/primitive.h"

namespace legend {
namespace primitive {

/**
 * @brief Box型のプリミティブ描画クラス
 */
class Box : public PrimitiveBase {
 public:
  /**
   * @brief コンストラクタ
   */
  Box();
  /**
   * @brief デストラクタ
   */
  virtual ~Box();
  /**
   * @brief 初期化
   * @return 初期化に成功したらtrueを返す
   */
  bool Init() override;
};

}  // namespace primitive
}  // namespace legend

#endif  //! LEGEND_PRIMITIVE_BOX_H_
