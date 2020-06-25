#ifndef LEGEND_SCENES_TITLE_H_
#define LEGEND_SCENES_TITLE_H_

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {

/**
* @class Title
* @brief タイトルシーン
*/
class Title : public Scene {
 public:
  /**
  * @brief コンストラクタ
  */
  Title();
  /**
   * @brief デストラクタ
   */
  virtual ~Title();
  /**
   * @brief 更新
   */
  virtual Scene *Update() override;
  /**
   * @brief 描画
   */
  virtual void Draw() const override;
};

} // namespace scenes
} // namespace legend

#endif //! LEGEND_SCENES_TITLE_H_
