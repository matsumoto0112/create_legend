#ifndef LEGEND_SEARCH_SEARCH_COURSE_H_
#define LEGEND_SEARCH_SEARCH_COURSE_H_

#include <vector>
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
  SearchCourse(SearchAI* _baseSearch, SearchCourse* _parentSearch);

  /**
   * @brief デスクトラクタ
   */
  ~SearchCourse();

  /**
   * @brief 原点取得
   */
  SearchAI* GetBaseSeach();
  /**
   * @brief 探索先設定
   */
  std::vector<SearchAI*> SetChild(std::vector<SearchAI*> rem);
  /**
   * @brief 原点までの分岐元取得
   */
  std::vector<SearchCourse*> GetParents();
  /**
   * @brief 分岐元の原点の取得
   */
  SearchCourse* GetMostParent();
  /**
   * @brief 分岐元の原点までの距離
   */
  float Length();

  private:
  std::vector<SearchCourse*> GetParents(std::vector<SearchCourse*> _parents);

 private:
  SearchAI* baseSearch_;
  SearchCourse* parentSearch_;
  std::vector<SearchAI*> childSearch_ = {};
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_COURSE_H_