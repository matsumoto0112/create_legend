#ifndef LEGEND_DIRECTX_FRAME_RESOURCE_H_
#define LEGEND_DIRECTX_FRAME_RESOURCE_H_

/**
 * @file frame_resource.h
 * @brief DirectX�f�o�C�X�ŊǗ�����t���[���P�ʂł̃R�}���h���X�g���\�[�X��`
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {

/**
 * @class FrameResource
 * @brief �t���[�����\�[�X
 */
class FrameResource {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  FrameResource();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~FrameResource();
  /**
   * @brief ������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor,
            D3D12_COMMAND_LIST_TYPE command_list_type);
  /**
   * @brief ���\�[�X�̏���������
   * @return ����������true��Ԃ�
   */
  bool Ready();
  /**
   * @brief ���\�[�X��j������
   */
  void Destroy();
  /**
   * @brief �R�}���h���X�g���擾����
   */
  device::CommandList& GetCommandList() { return command_lists_; }

 public:
  //! �t�F���X�l
  u64 fence_value_;

 private:
  //! �R�}���h���X�g
  device::CommandList command_lists_;
};

}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_FRAME_RESOURCE_H_
