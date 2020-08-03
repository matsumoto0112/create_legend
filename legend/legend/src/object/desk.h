#ifndef LEGEND_OBJECT_DESK_H_
#define LEGEND_OBJECT_DESK_H_

#include "src/directx/buffer/constant_buffer.h"
#include "src/draw/model.h"
#include "src/physics/bounding_box.h"
#include "src/util/transform.h"

namespace legend {
namespace object {

class Desk {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Desk();
  /**
   * @brief �R���X�g���N�^
   * @param ���W
   * @param ��]
   * @param �X�P�[��
   */
  Desk(math::Vector3 position, math::Quaternion rotation, math::Vector3 scale);
  /**
   * @brief �f�X�g���N�^
   */
  ~Desk();
  /**
   * @brief ������
   */
  bool Initialize(directx::DirectX12Device& device,
      util::resource::Resource& resource);
  /**
   * @brief �X�V
   */
  bool Update();
  /**
   * @brief �`��
   */
  void Draw(directx::DirectX12Device& device);
  /**
   * @brief ���W�̐ݒ�
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief ��]�̐ݒ�
   */
  void SetRotation(math::Quaternion rotation);
  /**
   * @brief �X�P�[���̐ݒ�
   */
  void SetScale(math::Vector3 scale);
  /**
   * @brief ���W�̎擾
   */
  math::Vector3 GetPosition();
  /**
   * @brief ��]�̎擾
   */
  math::Quaternion GetRotation();
  /**
   * @brief �X�P�[���̐ݒ�
   */
  math::Vector3 GetScale();
  /**
   * @brief �����̂̎擾
   */
  physics::BoundingBox& GetOBB();

 private:
  //! �Փ˔���p�̒�����
  physics::BoundingBox obb_;
  //! �g�����X�t�H�[���]���p�R���X�^���g�o�b�t�@
  directx::buffer::ConstantBuffer<directx::constant_buffer_structure::Transform>
      transform_cb_;
  //! �g�����X�t�H�[��
  util::Transform transform_;
};

}  // namespace object
}  // namespace legend

#endif  //! LEGEND_OBJECT_DESK_H_
