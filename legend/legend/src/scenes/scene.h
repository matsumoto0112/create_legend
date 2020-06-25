#ifndef LEGEND_SCENES_BASE_SCENE_H_
#define LEGEND_SCENES_BASE_SCENE_H_

namespace legend {
namespace scenes {

/**
* @class BaseScene
* @brief シーンの基底クラス
*/
class Scene {
 protected:
  /**
  * @brief コンストラクタ
  */
  Scene() {}

 public:
  /**
   * @brief コピーコンストラクタ禁止
   */
  Scene(const Scene &) = delete;
  /**
   * @brief コピーコンストラクタ禁止
   */
  Scene &operator=(const Scene &) = delete;

  /**
   * @brief 仮想デストラクタ
   */
  virtual ~Scene() = default;

 public:
  /**
   * @brief 更新
   */
  virtual Scene *Update() = 0;
  /**
   * @brief 描画
   */
  virtual void Draw() const = 0;
};

} // namespace scenes
} // namespace legend

#endif //! LEGEND_SCENES_BASE_SCENE_H_
