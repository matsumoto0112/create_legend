#ifndef LEGEND_SEARCH_SEARCH_COURSE_H_
#define LEGEND_SEARCH_SEARCH_COURSE_H_

#include <vector>
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
  SearchCourse(SearchAI* _baseSearch, SearchCourse* _parentSearch);

  /**
   * @brief �f�X�N�g���N�^
   */
  ~SearchCourse();

  /**
   * @brief ���_�擾
   */
  SearchAI* GetBaseSeach();
  /**
   * @brief �T����ݒ�
   */
  std::vector<SearchAI*> SetChild(std::vector<SearchAI*> rem);
  /**
   * @brief ���_�܂ł̕��򌳎擾
   */
  std::vector<SearchCourse*> GetParents();
  /**
   * @brief ���򌳂̌��_�̎擾
   */
  SearchCourse* GetMostParent();
  /**
   * @brief ���򌳂̌��_�܂ł̋���
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