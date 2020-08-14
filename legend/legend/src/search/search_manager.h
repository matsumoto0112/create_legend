#ifndef LEGEND_SEARCH_SEARCH_MANAGER_H_
#define LEGEND_SEARCH_SEARCH_MANAGER_H_

#include <vector>
#include "src/physics/ray.h"
#include "src/player/player.h"
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
   * @brief �����ݒ�
   */
  void Initialize(physics::BoundingBox* _player_obb);

  /**
   * @brief ���W�ǉ�
   */
  void Add(math::Vector3 _position);
  /**
   * @brief ���W�ǉ�
   */
  void Add(std::vector<math::Vector3> _positions);
  /**
   * @brief ����ǉ�
   */
  void SetBranch(i32 index, std::vector<i32> branch);

  /**
   * @brief ���̍��W
   */
  math::Vector3 NextSearch(physics::BoundingBox* _enemy,
                           std::vector<physics::BoundingBox*> _enemys);
  /**
   * @brief �o�H�T��
   */
  void SetCourse(SearchAI* sStart, SearchAI* sEnd);

  void DebugDraw(directx::device::CommandList& command_list);

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
  /**
   * @brief �ߏ�̒T���ӏ����擾
   */
  SearchAI* NearSearch(math::Vector3 _position);

  bool OnCollision(math::Vector3 start, math::Vector3 direction);

 private:
  std::vector<std::unique_ptr<SearchAI>> search_list_;
  std::vector<SearchAI*> course_list_;

  physics::BoundingBox* player_obb_;
  std::vector<physics::BoundingBox*> enemys_;
  physics::BoundingBox* ignore_enemy_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_MANAGER_H_