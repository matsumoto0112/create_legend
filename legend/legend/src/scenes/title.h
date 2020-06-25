#ifndef LEGEND_SCENES_TITLE_H_
#define LEGEND_SCENES_TITLE_H_

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {

/**
* @class Title
* @brief �^�C�g���V�[��
*/
class Title : public Scene {
 public:
  /**
  * @brief �R���X�g���N�^
  */
  Title();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Title();
  /**
   * @brief �X�V
   */
  virtual Scene *Update() override;
  /**
   * @brief �`��
   */
  virtual void Draw() const override;
};

} // namespace scenes
} // namespace legend

#endif //! LEGEND_SCENES_TITLE_H_
