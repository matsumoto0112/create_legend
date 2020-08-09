#ifndef LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
#define LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_

/**
 * @file swap_chain.h
 * @brief �X���b�v�`�F�C���N���X��`
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/render_target/render_target.h"

namespace legend {
namespace directx {
namespace device {
/**
 * @class SwapChain
 * @brief �X���b�v�`�F�C���N���X
 */
class SwapChain {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  SwapChain();
  /**
   * @brief �f�X�g���N�^
   */
  ~SwapChain();
  /**
   * @brief ������
   * @param
   * @return �������ɐ���������true��Ԃ�
   */

  /**
   * @brief ������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param adapter �A�_�v�^�[
   * @param frame_count �o�b�N�o�b�t�@�̃t���[����
   * @param width ��ʂ̕�
   * @param height ��ʂ̍���
   * @param format �o�b�N�o�b�t�@�̃t�H�[�}�b�g
   * @param hwnd �E�B���h�E�n���h��
   * @param command_queue �R�}���h�L���[
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, DXGIAdapter& adapter, u32 frame_count,
            u32 width, u32 height, DXGI_FORMAT format, HWND hwnd,
            ID3D12CommandQueue* command_queue);
  /**
   * @brief �o�b�N�o�b�t�@���擾����
   * @param index �o�b�N�o�b�t�@�̃C���f�b�N�X
   */
  ComPtr<ID3D12Resource> GetBuffer(u32 index) const;
  /**
   * @brief �`����e��\������
   * @return ����������true��Ԃ�
   */
  bool Present();
  /**
   * @brief ���݂̃o�b�N�o�b�t�@�̃C���f�b�N�X���擾����
   */
  u32 GetCurrentBackBufferIndex() const;

 public:
  //! �X���b�v�`�F�C��
  ComPtr<IDXGISwapChain3> swap_chain_;
  //! �e�A�����O��������Ă��邩
  bool allow_tearing_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
