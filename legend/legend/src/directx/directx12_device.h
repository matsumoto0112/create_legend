#ifndef LEGEND_DIRECTX_DIRECTX12_DEVICE_H_
#define LEGEND_DIRECTX_DIRECTX12_DEVICE_H_

/**
 * @file directx12_device.h
 * @brief DirectX12�f�o�C�X��`
 */

#include <wrl/wrappers/corewrappers.h>

#include "src/libs/d3dx12.h"
#include "src/window/window.h"

namespace legend {
namespace directx {
/**
 * @class DirectX12Device
 * @brief DirectX12�f�o�C�X
 */
class DirectX12Device {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  DirectX12Device();
  /**
   * @brief �f�X�g���N�^
   */
  ~DirectX12Device();

  /**
   * @brief ������
   * @param target_window �`��Ώۂ̃E�B���h�E
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(std::shared_ptr<window::Window> target_window);

  void Prepare();
  void Present();

 public:
  ID3D12Device* GetDevice() const { return device_.Get(); }
  ID3D12GraphicsCommandList5* GetCommandList() const {
    return command_list_.Get();
  }

 private:
  bool CreateDevice();
  ComPtr<IDXGIAdapter1> GetHardwareAdapter();
  void MoveToNextFrame();

 private:
  //! �g���v���o�b�t�@���g�p����
  static constexpr unsigned int FRAME_COUNT = 3;

 private:
  //! �����_�[�^�[�Q�b�g�ƂȂ�E�B���h�E
  std::weak_ptr<window::Window> target_window_;
  //! �����_�[�^�[�Q�b�g�ƂȂ�X�N���[���̑傫��
  math::IntVector2 render_target_screen_size_;
  //! DXGI�t�@�N�g��
  ComPtr<IDXGIFactory4> factory_;
  //! D3D12�f�o�C�X
  ComPtr<ID3D12Device> device_;
  //! �R�}���h�L���[
  ComPtr<ID3D12CommandQueue> command_queue_;
  //! �X���b�v�`�F�C��
  ComPtr<IDXGISwapChain4> swap_chain_;
  //! �o�b�t�@�C���f�b�N�X
  unsigned int frame_index_;
  //! �����_�[�^�[�Q�b�g�̃f�B�X�N���v�^�q�[�v
  ComPtr<ID3D12DescriptorHeap> rtv_heap_;
  //! �����_�[�^�[�Q�b�g�̃��\�[�X
  std::array<ComPtr<ID3D12Resource>, FRAME_COUNT> render_targets_;
  //! �����_�[�^�[�Q�b�g�̃q�[�v�T�C�Y
  unsigned int rtv_heap_size_;
  //! �R�}���h�A���P�[�^
  std::array<ComPtr<ID3D12CommandAllocator>, FRAME_COUNT> command_allocator_;
  //! �R�}���h�t�F���X
  ComPtr<ID3D12Fence> fence_;
  ComPtr<ID3D12GraphicsCommandList5> command_list_;
  D3D12_RESOURCE_STATES current_resource_state_;
  //! �t�F���X�l
  std::array<unsigned long long, FRAME_COUNT> fence_values_;
  //! �t�F���X�C�x���g
  Microsoft::WRL::Wrappers::Event fence_event_;
};
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX12_DEVICE_H_
