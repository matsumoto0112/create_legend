#ifndef LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
#define LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_

/**
 * @file swap_chain.h
 * @brief �X���b�v�`�F�C���N���X��`
 */

#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/render_target/render_target.h"
#include "src/window/window.h"

namespace legend {
namespace directx {
namespace device {
/**
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
   * @param accessor DirectX12�A�N�Z�T
   * @param apapter �A�_�v�^�[
   * @param target_window �`��Ώۂ̃E�B���h�Ek
   * @param format �o�b�N�o�b�t�@�̃t�H�[�}�b�g
   * @param back_buffer_count �o�b�N�o�b�t�@�̖���
   * @param command_queue �R�}���h�L���[
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(ID3D12Device& accessor, DXGIAdapter& adapter,
            window::Window& target_window, DXGI_FORMAT format,
            u32 back_buffer_count, ID3D12CommandQueue* command_queue);
  /**
   * @brief �o�b�N�o�b�t�@���N���A����
   * @param accessor DirectX12�A�N�Z�T
   */
  void ClearBackBuffer(IDirectXAccessor& accessor);
  /**
   * @brief �`��J�n
   * @param accessor DirectX12�A�N�Z�T
   */
  void DrawBegin(IDirectXAccessor& accessor);
  /**
   * @brief �`��I��
   * @param accessor DirectX12�A�N�Z�T
   */
  void DrawEnd(IDirectXAccessor& accessor);
  /**
   * @brief �o�b�N�o�b�t�@��\������
   * @return �\���ɐ���������true��Ԃ�
   */
  bool Present();
  /**
   * @brief �r���[�|�[�g���Z�b�g����
   * @param accessor DirectX12�A�N�Z�T
   */
  void SetViewport(IDirectXAccessor& accessor) const;
  /**
   * @brief �V�U�[��`���Z�b�g����
   * @param accessor DirectX12�A�N�Z�T
   */
  void SetScissorRect(IDirectXAccessor& accessor) const;

 public:
  /**
   * @brief ���݂̃����_�[�^�[�Q�b�g���擾����
   */
  const render_target::RenderTarget& GetRenderTarget() const {
    return render_targets_[GetCurrentFrameIndex()];
  }
  /**
   * @brief ���݂̃t���[���C���f�b�N�X���擾����
   */
  u32 GetCurrentFrameIndex() const;

 private:
  //! �X���b�v�`�F�C��
  ComPtr<IDXGISwapChain3> swap_chain_;
  //! �����_�[�^�[�Q�b�g
  std::vector<render_target::RenderTarget> render_targets_;
  //�e�A�����O��������Ă��邩
  bool allow_tearing_;
  D3D12_VIEWPORT viewport_;
  D3D12_RECT scissor_rect_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
