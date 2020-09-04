#ifndef LEGEND_TURN_CHANGE_TURN_CHANGE_H_
#define LEGEND_TURN_CHANGE_TURN_CHANGE_H_
#include "src/draw/sprite_renderer.h"
#include "src/math/vector_2.h"
#include "src/math/math_util.h"
#include "src/system/mode.h"

namespace legend {
namespace system {
class TurnSystem;
}  // namespace system
namespace turn_change {

class TurnChange {
 public:
  TurnChange();
  ~TurnChange();
  bool Init(system::TurnSystem* turn_system);
  bool ChangeStart(system::Mode next_mode);
  bool Update();
  void Draw();

private:
    math::Vector2 LerpVector2(math::Vector2 v1, math::Vector2 v2, float t);

 private:
  //! �\�������Ԃ��ǂ���
  bool is_view_;
  //! �^�C�}�[
  float timer_;
  //! �E�B���h�E�T�C�Y
  math::Vector2 screen_size_;
  //! �^�[���V�X�e���̃|�C���^
  system::TurnSystem* turn_system_;
  //! �ύX�O�̃^�[���摜
  draw::Sprite2D before_turn_sprite_;
  //! �ύX��̃^�[���摜
  draw::Sprite2D next_turn_sprite_;
  //! �ύX��̃��[�h
  system::Mode next_mode_;
};

}  // namespace turn_change
}  // namespace legend
#endif  //! LEGEND_TURN_CHANGE_TURN_CHANGE_H_
