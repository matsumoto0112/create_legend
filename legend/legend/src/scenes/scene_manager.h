#ifndef LEGEND_SCENES_SCENE_MANAGER_H_
#define LEGEND_SCENES_SCENE_MANAGER_H_

namespace legend {
namespace scenes {

class Scene;
/**
 * @class SceneManager
 * @brief �V�[���Ǘ�
 */
class SceneManager {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  SceneManager();
  /**
   * @brief �R�s�[�R���X�g���N�^�֎~
   */
  SceneManager(const SceneManager &) = delete;
  /**
   * @brief �R�s�[�R���X�g���N�^�֎~
   */
  SceneManager &operator=(const SceneManager &) = delete;
  /**
   * @brief �f�X�g���N�^
   */
  ~SceneManager();

 private:
  Scene *scene_;

 public:
   /**
   * @brief �X�V
   */
  void Update();
  /**
   * @brief �V�[���̒ǉ�
   */
  void Add();
  /**
   * @brief �V�[���J��
   */
  void Change();
  /**
   * @brief �`��
   */
  void Draw() const;
};

}  // namespace scenes
}  // namespace legend

#endif //! LEGEND_SCENES_SCENE_MANAGER_H_
