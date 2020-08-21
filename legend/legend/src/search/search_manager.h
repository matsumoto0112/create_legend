#ifndef LEGEND_SEARCH_SEARCH_MANAGER_H_
#define LEGEND_SEARCH_SEARCH_MANAGER_H_

#include "src/bullet/physics_field.h"
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
  void Initialize(actor::IActorMediator* mediator);

  /**
   * @brief 生成
   */
  void Make(std::filesystem::path filepath);
  /**
   * @brief テキストの読み込み処理
   * @return 処理が正しく終了したら読み込まれた内容(string)を返す
   */
  std::vector<std::string> LoadStringStageData(std::filesystem::path filepath);
  /**
   * @brief StringをStringのリストへ変換
   * @param x,y,z 元文字列
   * @return 変換されたStringのリスト
   */
  std::vector<std::string> StringSplit(const std::string& string, char border);


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
  math::Vector3 NextSearch(bullet::Collider* _enemy,
                           std::vector<bullet::Collider*> _enemys);
  /**
   * @brief 経路探索
   */
  void SetCourse(SearchAI* sStart, SearchAI* sEnd);

  void DebugDraw(bullet::PhysicsField* physics_field_);

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

  std::vector<bullet::Collider*> enemys_;
  bullet::Collider* ignore_enemy_;

  actor::IActorMediator* mediator_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_MANAGER_H_