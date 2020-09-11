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
   * @param _baseSearch 原点
   */
  SearchCourse(SearchAI* _baseSearch);
  /**
   * @brief コンストラクタ
   * @param _baseSearch 原点
   * @param _parentSearch 分岐親番号
   */
  SearchCourse(SearchAI* _baseSearch, i32 _parentSearch);

  /**
   * @brief デスクトラクタ
   */
  ~SearchCourse();

  /**
   * @brief 原点取得
   * @return 原点取得
   */
  SearchAI* GetBaseSearch();
  /**
   * @brief 探索先設定
   * @param children 子供リスト
   * @param remove 削除対象リスト
   */
  void SetChild(std::vector<SearchCourse>& children,
                std::vector<SearchCourse>& remove);
  /**
   * @brief 原点までの分岐元取得
   * @param searched 探索済みリスト
   */
  std::vector<i32> GetParents(std::vector<SearchCourse>& searched);
  /**
   * @brief 分岐元の原点までの距離
   * @param searched 探索済みリスト
   */
  float Length(std::vector<SearchCourse>& searched);

 private:
  /**
   * @brief 分岐元の原点までの距離
   * @param searched 探索済みリスト
   * @param _parents 経路元リスト
   * @return 原点までの経路番号リスト
   */
  std::vector<i32> GetParents(std::vector<SearchCourse>& searched,
                              std::vector<i32>& _parents);

 private:
  //! 原点
  SearchAI* baseSearch_;
  //! 経路親元番号
  i32 parentSearch_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_COURSE_H_