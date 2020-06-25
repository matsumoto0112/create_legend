#ifndef LEGEND_SCENES_BASE_SCENE_H_
#define LEGEND_SCENES_BASE_SCENE_H_

namespace legend {
namespace scenes {

/**
* @class BaseScene
* @brief �V�[���̊��N���X
*/
class Scene {
 protected:
  /**
  * @brief �R���X�g���N�^
  */
  Scene() {}

 public:
  /**
   * @brief �R�s�[�R���X�g���N�^�֎~
   */
  Scene(const Scene &) = delete;
  /**
   * @brief �R�s�[�R���X�g���N�^�֎~
   */
  Scene &operator=(const Scene &) = delete;

  /**
   * @brief ���z�f�X�g���N�^
   */
  virtual ~Scene() = default;

 public:
  /**
   * @brief �X�V
   */
  virtual Scene *Update() = 0;
  /**
   * @brief �`��
   */
  virtual void Draw() const = 0;
};

} // namespace scenes
} // namespace legend

#endif //! LEGEND_SCENES_BASE_SCENE_H_
