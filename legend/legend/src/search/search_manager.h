#ifndef LEGEND_SEARCH_SEARCH_MANAGER_H_
#define LEGEND_SEARCH_SEARCH_MANAGER_H_

#include <vector>
#include "src/search/search_course.h"

namespace legend {
namespace search {

/**
 * @class SearchManager
 * @brief 探索管理クラス
 */
class SearchManager {
 public:
  /**
   * @brief コンストラクタ
   */
  SearchManager();
  /**
   * @brief デスクトラクタ
   */
  ~SearchManager();

  /**
   * @brief 次の座標
   */
  math::Vector3 NextSearch(math::Vector3 _position);
  /**
   * @brief 経路探索
   */
  void SetCourse(SearchAI* sStart, SearchAI* sEnd);

 private:
  /**
   * @brief 追跡経路
   */
  void ChaseCourse();
  /**
   * @brief 次の経路
   */
  math::Vector3 NextCourse(math::Vector3 _position);
  /**
   * @brief 分岐先設定
   */
  std::vector<SearchCourse*> SetChild(SearchCourse* course,
                                      std::vector<SearchCourse*> searched);
  /**
   * @brief 探索箇所ランダムで取得
   */
  SearchAI* GetRandomSearch(std::vector<SearchAI*> remove);

 private:
  std::vector<SearchAI*> searchList;
  std::vector<SearchAI*> courseList;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_MANAGER_H_