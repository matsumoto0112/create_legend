#ifndef LEGEND_SEARCH_SEARCH_COURSE_H_
#define LEGEND_SEARCH_SEARCH_COURSE_H_

#include <vector>
#include "src/actor/actor_mediator.h"
#include "src/search/search_ai.h"

namespace legend {
namespace search {

/**
 * @class SearchCourse
 * @brief 探索用クラス
 */
struct SearchCourse {
 public:
  /**
   * @brief コンストラクタ
   */
  SearchCourse(SearchAI* _baseSearch);
  /**
   * @brief コンストラクタ
   */
  SearchCourse(SearchAI* _baseSearch, i32 _parentSearch);

  /**
   * @brief デスクトラクタ
   */
  ~SearchCourse();

  /**
   * @brief 原点取得
   */
  SearchAI* GetBaseSearch();
  /**
   * @brief 探索先設定
   */
  void SetChild(std::vector<SearchCourse>& children,
                std::vector<SearchCourse>& remove);
  /**
   * @brief 原点までの分岐元取得
   */
  std::vector<i32> GetParents(std::vector<SearchCourse>& searched);
  /**
   * @brief 分岐元の原点までの距離
   */
  float Length(std::vector<SearchCourse>& searched);

 private:
  std::vector<i32> GetParents(std::vector<SearchCourse>& searched,
                              std::vector<i32>& _parents);

 private:
  SearchAI* baseSearch_;
  i32 parentSearch_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_COURSE_H_