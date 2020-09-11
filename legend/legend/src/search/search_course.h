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
   * @param _baseSearch ���_
   */
  SearchCourse(SearchAI* _baseSearch);
  /**
   * @brief �R���X�g���N�^
   * @param _baseSearch ���_
   * @param _parentSearch ����e�ԍ�
   */
  SearchCourse(SearchAI* _baseSearch, i32 _parentSearch);

  /**
   * @brief �f�X�N�g���N�^
   */
  ~SearchCourse();

  /**
   * @brief ���_�擾
   * @return ���_�擾
   */
  SearchAI* GetBaseSearch();
  /**
   * @brief �T����ݒ�
   * @param children �q�����X�g
   * @param remove �폜�Ώۃ��X�g
   */
  void SetChild(std::vector<SearchCourse>& children,
                std::vector<SearchCourse>& remove);
  /**
   * @brief ���_�܂ł̕��򌳎擾
   * @param searched �T���ς݃��X�g
   */
  std::vector<i32> GetParents(std::vector<SearchCourse>& searched);
  /**
   * @brief ���򌳂̌��_�܂ł̋���
   * @param searched �T���ς݃��X�g
   */
  float Length(std::vector<SearchCourse>& searched);

 private:
  /**
   * @brief ���򌳂̌��_�܂ł̋���
   * @param searched �T���ς݃��X�g
   * @param _parents �o�H�����X�g
   * @return ���_�܂ł̌o�H�ԍ����X�g
   */
  std::vector<i32> GetParents(std::vector<SearchCourse>& searched,
                              std::vector<i32>& _parents);

 private:
  //! ���_
  SearchAI* baseSearch_;
  //! �o�H�e���ԍ�
  i32 parentSearch_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_COURSE_H_