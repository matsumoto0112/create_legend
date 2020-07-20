#ifndef LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
#define LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_

/**
 * @file swap_chain.h
 * @brief �X���b�v�`�F�C���N���X��`

 */

#include "src/directx/render_target/render_target.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/window/window.h"

namespace legend {
namespace directx {
namespace device {
/**
 * @brief �X���b�v�`�F�C���N���X
 */
class SwapChain {
  //! �o�b�N�o�b�t�@�̖���
  static constexpr u32 FRAME_COUNT = 3;

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
   * @param command_queue �R�}���h�L���[
   * @param format �o�b�N�o�b�t�@�̃t�H�[�}�b�g
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, DXGIAdapter& adapter,
            window::Window& target_window, ID3D12CommandQueue* command_queue,
            DXGI_FORMAT format);
  /**
   * @brief �o�b�N�o�b�t�@���Z�b�g����
   * @param accessor DirectX12�A�N�Z�T
   */
  void SetBackBuffer(IDirectXAccessor& accessor);
  /**
   * @brief �o�b�N�o�b�t�@���N���A����
   * @param accessor DirectX12�A�N�Z�T
   */
  void ClearBackBuffer(IDirectXAccessor& accessor);
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

 public:
  /**
   * @brief ���݂̃����_�[�^�[�Q�b�g���擾����
   */
  const buffer::RenderTarget& GetRenderTarget() const {
    return render_targets_[frame_index_];
  }
  /**
   * @brief ���݂̃t���[���C���f�b�N�X���擾����
   */
  u32 GetCurrentFrameIndex() const { return frame_index_; };
  /**
   * @brief �t���[���C���f�b�N�X���X�V����
   */
  void UpdateCurrentFrameIndex();

 private:
  //! �X���b�v�`�F�C��
  ComPtr<IDXGISwapChain3> swap_chain_;
  //! �����_�[�^�[�Q�b�g
  std::array<buffer::RenderTarget, FRAME_COUNT> render_targets_;
  //! ���݃t���[���̃C���f�b�N�X
  u32 frame_index_;
  //�e�A�����O��������Ă��邩
  bool allow_tearing_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_SWAP_CHAIN_H_
