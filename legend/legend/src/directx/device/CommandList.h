#ifndef LEGEND_DIRECTX_DEVICE_COMMANDLIST_H_
#define LEGEND_DIRECTX_DEVICE_COMMANDLIST_H_

/**
 * @file CommandList.h
 * @brief �R�}���h���X�g�N���X��`
 */

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
   * @param device �f�o�C�X
   * @param command_queue �R�}���h�L���[
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(ID3D12Device* device, ID3D12CommandQueue* command_queue);
  /**
   * @brief �R�}���h���X�g�����
   * @return ���邱�Ƃɐ���������true��Ԃ�
   */
  bool Close();

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

#endif  //! LEGEND_DIRECTX_DEVICE_COMMANDLIST_H_
