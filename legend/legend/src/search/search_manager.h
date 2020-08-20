#ifndef LEGEND_SEARCH_SEARCH_MANAGER_H_
#define LEGEND_SEARCH_SEARCH_MANAGER_H_

#include "src/player/player.h"
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
   * @brief 初期設定
   */
  void Initialize(bullet::Collider* _player_obb);

  /**
   * @brief 座標追加
   */
  void Add(math::Vector3 _position);
  /**
   * @brief 座標追加
   */
  void Add(std::vector<math::Vector3> _positions);
  /**
   * @brief 分岐追加
   */
  void SetBranch(i32 index, std::vector<i32> branch);

  /**
   * @brief 次の座標
   */
  math::Vector3 NextSearch(bullet::BoundingBox* _enemy,
                           std::vector<bullet::BoundingBox*> _enemys);
  /**
   * @brief 経路探索
   */
  void SetCourse(SearchAI* sStart, SearchAI* sEnd);

  void DebugDraw(directx::device::CommandList& command_list);

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
  /**
   * @brief 近場の探索箇所を取得
   */
  SearchAI* NearSearch(math::Vector3 _position);

  bool OnCollision(math::Vector3 start, math::Vector3 direction);

 private:
  std::vector<std::unique_ptr<SearchAI>> search_list_;
  std::vector<SearchAI*> course_list_;

  bullet::Collider* player_collider_;
  std::vector<bullet::BoundingBox*> enemys_;
  bullet::BoundingBox* ignore_enemy_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_MANAGER_H_