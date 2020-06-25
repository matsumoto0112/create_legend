#ifndef LEGEND_SCENES_SCENE_MANAGER_H_
#define LEGEND_SCENES_SCENE_MANAGER_H_

namespace legend {
namespace scenes {
/**
 * @class SceneManager
 * @brief シーン管理
 */
class SceneManager {
 public:
  /**
   * @brief コンストラクタ
   */
  SceneManager();
  /**
   * @brief コピーコンストラクタ禁止
   */
  SceneManager(const SceneManager &) = delete;
  /**
   * @brief コピーコンストラクタ禁止
   */
  SceneManager &operator=(const SceneManager &) = delete;
  /**
   * @brief デストラクタ
   */
  ~SceneManager();

   /**
   * @brief 更新
   */
  void Update();
  /**
   * @brief シーンの追加
   */
  void Add();
  /**
   * @brief シーン遷移
   */
  void Change();
  /**
   * @brief 描画
   */
  void Draw() const;

 private:

};

}  // namespace scenes
}  // namespace legend

#endif //! LEGEND_SCENES_SCENE_MANAGER_H_
