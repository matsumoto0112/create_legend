#ifndef LEGEND_SEARCH_SEARCH_COURSE_H_
#define LEGEND_SEARCH_SEARCH_COURSE_H_

#include <vector>
#include "src/actor/actor_mediator.h"
#include "src/search/search_ai.h"

namespace legend {
namespace search {

/**
 * @class SearchCourse
 * @brief �T���p�N���X
 */
struct SearchCourse {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  SearchCourse(SearchAI* _baseSearch);
  /**
   * @brief �R���X�g���N�^
   */
  SearchCourse(SearchAI* _baseSearch, i32 _parentSearch);

  /**
   * @brief �f�X�N�g���N�^
   */
  ~SearchCourse();

  /**
   * @brief ���_�擾
   */
  SearchAI* GetBaseSearch();
  /**
   * @brief �T����ݒ�
   */
  void SetChild(std::vector<SearchCourse>& children,
                std::vector<SearchCourse>& remove);
  /**
   * @brief ���_�܂ł̕��򌳎擾
   */
  std::vector<i32> GetParents(std::vector<SearchCourse>& searched);
  /**
   * @brief ���򌳂̌��_�܂ł̋���
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