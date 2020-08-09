#ifndef LEGEND_PRIMITIVE_LINE_H_
#define LEGEND_PRIMITIVE_LINE_H_

/**
 * @file line.h
 * @brief Line型のプリミティブ描画クラス定義
 */

#include "src/primitive/primitive.h"

namespace legend {
namespace primitive {

/**
 * @brief Line型のプリミティブ描画クラス
 */
class Line : public PrimitiveBase {
 public:
  /**
   * @brief コンストラクタ
   */
  Line();
  /**
   * @brief デストラクタ
   */
  virtual ~Line();
  /**
   * @brief 初期化
   * @return 初期化に成功したらtrueを返す
   */
  bool Init() override;

 private:
};

}  // namespace primitive
}  // namespace legend
#endif  //! LEGEND_PRIMITIVE_LINE_H_
