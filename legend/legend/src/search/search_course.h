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
  SearchCourse(actor::IActorMediator* _mediator, SearchAI* _baseSearch);
  /**
   * @brief �R���X�g���N�^
   */
  SearchCourse(actor::IActorMediator* _mediator, SearchAI* _baseSearch,
               SearchCourse* _parentSearch);

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
  void SetChild(std::vector<SearchCourse>& children,
                std::vector<SearchCourse>& remove, actor::Actor* ignore);
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
  const float Length();

 private:
  std::vector<SearchCourse*> GetParents(std::vector<SearchCourse*> _parents);

  bool OnCollision(actor::Actor* ignore, math::Vector3 start,
                   math::Vector3 direction);

 private:
  SearchAI* baseSearch_;
  SearchCourse* parentSearch_;
  std::vector<SearchCourse> childSearch_ = {};

  actor::IActorMediator* mediator_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_COURSE_H_