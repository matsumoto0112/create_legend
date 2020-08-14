#ifndef LEGEND_SEARCH_SEARCH_MANAGER_H_
#define LEGEND_SEARCH_SEARCH_MANAGER_H_

#include <vector>
#include "src/search/search_course.h"

namespace legend {
namespace search {

/**
 * @class SearchManager
 * @brief �T���Ǘ��N���X
 */
class SearchManager {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  SearchManager();
  /**
   * @brief �f�X�N�g���N�^
   */
  ~SearchManager();

  /**
   * @brief ���̍��W
   */
  math::Vector3 NextSearch(math::Vector3 _position);
  /**
   * @brief �o�H�T��
   */
  void SetCourse(SearchAI* sStart, SearchAI* sEnd);

 private:
  /**
   * @brief �ǐՌo�H
   */
  void ChaseCourse();
  /**
   * @brief ���̌o�H
   */
  math::Vector3 NextCourse(math::Vector3 _position);
  /**
   * @brief �����ݒ�
   */
  std::vector<SearchCourse*> SetChild(SearchCourse* course,
                                      std::vector<SearchCourse*> searched);
  /**
   * @brief �T���ӏ������_���Ŏ擾
   */
  SearchAI* GetRandomSearch(std::vector<SearchAI*> remove);

 private:
  std::vector<SearchAI*> searchList;
  std::vector<SearchAI*> courseList;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_MANAGER_H_