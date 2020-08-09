#ifndef LEGEND_DIRECTX_DEVICE_COMMAND_LIST_H_
#define LEGEND_DIRECTX_DEVICE_COMMAND_LIST_H_

/**
 * @file CommandList.h
 * @brief �R�}���h���X�g�N���X��`
 */

#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace device {
/**
 * @brief �R�}���h���X�g�N���X
 */
class CommandList {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  CommandList();
  /**
   * @brief �f�X�g���N�^
   */
  ~CommandList();
  /**
   * @brief ������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param type �R�}���h���X�g�̎��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, D3D12_COMMAND_LIST_TYPE type);
  /**
   * @brief �R�}���h���X�g�����
   * @return ���邱�Ƃɐ���������true��Ԃ�
   */
  bool Close();
  /**
   * @brief �R�}���h���X�g�����Z�b�g����
   * @return ���Z�b�g�ɐ���������true��Ԃ�
   */
  bool Reset();

 public:
  /**
   * @brief �R�}���h���X�g���擾����
   */
  inline ID3D12GraphicsCommandList4* GetCommandList() const {
    return command_list_.Get();
  }
  /**
   * @brief �R�}���h�A���P�[�^���擾����
   */
  inline ID3D12CommandAllocator* GetCommandAllocator() const {
    return command_allocator_.Get();
  }

 private:
  //! �A���P�[�^
  ComPtr<ID3D12CommandAllocator> command_allocator_;
  //! �R�}���h���X�g
  ComPtr<ID3D12GraphicsCommandList4> command_list_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_COMMAND_LIST_H_
