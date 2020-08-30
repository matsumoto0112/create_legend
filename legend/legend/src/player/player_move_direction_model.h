#ifndef LEGEND_PLAYER_PLAYER_MOVE_DIRECTION_MODEL_H
#define LEGEND_PLAYER_PLAYER_MOVE_DIRECTION_MODEL_H

/**
 * @file player_move_direction_model.h
 * @brief �v���C���[�̌����Ă��������`�悷�郂�f���Ǘ��N���X
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/util/transform.h"

namespace legend {
namespace player {

/**
 * @brief �v���C���[�̌����Ă��������`�悷�郂�f��
 */
class PlayerMoveDirectionModel {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PlayerMoveDirectionModel();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~PlayerMoveDirectionModel();
  /**
   * @brief ������
   */
  bool Init();
  /**
   * @brief ���W��ݒ肷��
   */
  void SetPosition(const math::Vector3& position);
  /**
   * @brief ��]��ݒ肷��
   */
  void SetRotation(const math::Quaternion& rotation);
  /**
   * @brief �`��
   */
  void Draw();

 private:
  //! �g�����X�t�H�[��
  util::Transform transform_;
  //! �g�����X�t�H�[���R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_cb_;
};
}  // namespace player
}  // namespace legend

#endif  //! LEGEND_PLAYER_PLAYER_MOVE_DIRECTION_MODEL_H
