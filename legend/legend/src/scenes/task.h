#ifndef LEGEND_SCENES_TASK_H_
#define LEGEND_SCENES_TASK_H_

namespace legend {
namespace scenes {

/**
 * @class Task
 * @brief �V�[���֌W�S�Ă̊��N���X
 */
class Task {
 public:
  virtual ~Task() {}
  virtual void Initialize() {}
  virtual void Finalize() {}
  virtual void Update() = 0;
  virtual void Draw() = 0;
};
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_TASK_H_
