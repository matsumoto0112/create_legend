#ifndef LEGEND_SEARCH_SEARCH_AI_H_
#define LEGEND_SEARCH_SEARCH_AI_H_

#include <vector>
#include "src/math/vector_3.h"

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
   */
  SearchAI(math::Vector3 _position);
  /**
   * @brief デスクトラクタ
   */
  ~SearchAI();

  /**
   * @brief 座標取得
   */
  math::Vector3 GetPosition();
  /**
   * @brief 分岐取得
   */
  std::vector<SearchAI*> GetBranch();
  /**
   * @brief 分岐設定
   */
  std::vector<SearchAI*> SetBranch(std::vector<SearchAI*> _branch);
  /**
   * @brief 分岐先からランダムで取得
   */
  SearchAI* GetRandomSearch(std::vector<SearchAI*> remove);
 private:
  std::vector<SearchAI*> branch_;
  math::Vector3 position_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_AI_H_