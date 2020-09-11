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
   * @param mediator 仲介者を追加
   */
  void Initialize(actor::IActorMediator* mediator);

  /**
   * @brief 生成
   * @param filepath ファイルパスを入力
   */
  void Make(std::filesystem::path filepath);
  /**
   * @brief テキストの読み込み処理
   * @param filepath ファイルパスを入力
   * @return 処理が正しく終了したら読み込まれた内容(string)を返す
   */
  std::vector<std::string> LoadStringStageData(std::filesystem::path filepath);
  /**
   * @brief StringをStringリストへ変換
   * @param string 入力String
   * @param border 折り返し箇所
   * @return 変換されたStringのリスト
   */
  std::vector<std::string> StringSplit(const std::string& string, char border);

  /**
   * @brief 座標から分岐点追加
   * @param _position 座標
   */
  void Add(math::Vector3 _position);
  /**
   * @brief 座標リストから分岐点追加
   * @param _positions 座標
   */
  void Add(std::vector<math::Vector3> _positions);
  /**
   * @brief 座標から分岐点追加
   * @param index 列番号
   * @param branch 追加する分岐先の列番号
   */
  void SetBranch(i32 index, std::vector<i32> branch);

  /**
   * @brief 次の経路座標
   * @param _enemy 現在稼働させる敵のコライダ
   * @param _enemys 全体の敵のコライダ
   * @return 次の移動先取得
   */
  math::Vector3 NextSearch(bullet::Collider* _enemy,
                           std::vector<bullet::Collider*> _enemys);
  /**
   * @brief 経路探索
   * @param sStart 開始地点
   * @param sEnd 終了地点
   */
  void SetCourse(SearchAI* sStart, SearchAI* sEnd);
  /**
   * @brief デバッグ描画
   * @param physics_field_ 物理演算クラス
   */
  void DebugDraw(bullet::PhysicsField* physics_field_);

 private:
  /**
   * @brief 追跡経路
   */
  void ChaseCourse();
  /**
   * @brief 座標から近場の探索箇所を取得
   * @param _position 座標
   * @return 座標から近場の探索箇所を取得
   */
  SearchAI* NearSearch(math::Vector3 _position);
  /**
   * @brief 原点のポインタを取得
   * @param _search 探索中の経路元を取得
   * @return 原点のポインタを取得
   */
  SearchAI* GetBaseSearch(SearchAI* _search);
  /**
   * @brief 衝突情報を取得
   * @param start 始点
   * @param direction 向き
   * @return 衝突しているか判定
   */
  bool OnCollision(math::Vector3 start, math::Vector3 direction);

 private:
  //! 探索箇所リスト
  std::vector<std::unique_ptr<SearchAI>> search_list_;
  //! 経路
  std::vector<SearchAI*> course_list_;

  //! 全体の敵一覧
  std::vector<bullet::Collider*> enemys_;
  //! 実行中の敵
  bullet::Collider* ignore_enemy_;

  //! 仲介者
  actor::IActorMediator* mediator_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_MANAGER_H_