#ifndef LEGEND_SEARCH_SEARCH_AI_H_
#define LEGEND_SEARCH_SEARCH_AI_H_

#include <vector>
#include "src/bullet/physics_field.h"
#include "src/math/vector_3.h"
#include "src/game/game_device.h"
#include "src/primitive/line.h"
#include "src/primitive/box.h"

namespace legend {
namespace search {

/**
 * @class SearchAI
 * @brief 分岐先保持クラス
 */
class SearchAI {
 public:
  /**
   * @brief コンストラクタ
   * @param _position 座標
   */
  SearchAI(math::Vector3 _position);
  /**
   * @brief デスクトラクタ
   */
  ~SearchAI();

  /**
   * @brief 座標取得
   * @return 座標取得
   */
  math::Vector3 GetPosition();
  /**
   * @brief 分岐リスト取得
   * @return 分岐リスト取得
   */
  std::vector<SearchAI*> GetBranch();
  /**
   * @brief 分岐設定
   * @param _branch 分岐先設定
   * @return 分岐設定
   */
  std::vector<SearchAI*> SetBranch(std::vector<SearchAI*> _branch);
  /**
   * @brief 分岐追加
   * @param _branch 分岐先追加
   * @return 追加後分岐リスト
   */
  std::vector<SearchAI*> AddBranch(SearchAI* _branch);
  /**
   * @brief 分岐先からランダムで取得
   * @param remove 検索対象外リスト
   * @return 分岐先からランダムで取得
   */
  SearchAI* GetRandomSearch(std::vector<SearchAI*> remove);
  /**
   * @brief デバッグ描画
   * @param physics_field_ 物理演算クラス
   */
  void DebugDraw(bullet::PhysicsField* physics_field_);
 private:
  //! 分岐先
  std::vector<SearchAI*> branch_;
  //! デバッグ描画用ライン
  std::vector<std::unique_ptr<primitive::Line>> lines_;
  //! デバッグ描画用ボックス
  std::unique_ptr<primitive::Box> box_;
  //! 座標
  math::Vector3 position_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_AI_H_