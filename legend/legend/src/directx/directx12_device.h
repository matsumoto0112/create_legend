#ifndef LEGEND_DIRECTX_DIRECTX12_DEVICE_H_
#define LEGEND_DIRECTX_DIRECTX12_DEVICE_H_

/**
 * @file directx12_device.h
 * @brief DirectX12�f�o�C�X��`
 */

#include <wrl/wrappers/corewrappers.h>

#include "src/directx/buffer/render_target.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/heap_manager.h"
#include "src/libs/d3dx12.h"
#include "src/window/window.h"

namespace legend {
namespace directx {

/**
 * @class DirectX12Device
 * @brief DirectX12�f�o�C�X
 */
class DirectX12Device : public IDirectXAccessor {
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
  /**
   * @brief ��������̏���
   * @return ���̏����ɐ���������true��Ԃ�
   */
  bool InitAfter();
  /**
   * @brief �`�揀��
   * @return ����������true��Ԃ�
   */
  bool Prepare();
  /**
   * @brief �`����e����ʂɉf��
   * @return ����������true��Ԃ�
   */
  bool Present();

  void SetBackBuffer();

  /**
   * @brief GPU�̏�����ҋ@����
   */
  void WaitForGPU() noexcept;

  virtual DescriptorHandle GetHandle(DescriptorHeapType heap_type) override;
  virtual void SetToGlobalHeap(u32 register_num, ResourceType resource_type,
                               const DescriptorHandle& handle) override;

 public:
  virtual ID3D12Device* GetDevice() const override { return device_.Get(); }
  virtual ID3D12GraphicsCommandList4* GetCommandList() const override {
    return command_list_.Get();
  }
  const buffer::RenderTarget& GetRenderTarget() const {
    return render_targets_[frame_index_];
  }
  /**
   * @brief �f�B�X�N���v�^�q�[�v�Ǘ��҂��擾����
   */
  HeapManager& GetHeapManager() { return heap_manager_; }

 private:
  /**
   * @brief �f�o�C�X�̍쐬
   * @return ����������true��Ԃ�
   */
  bool CreateDevice();
  /**
   * @brief ���̃t���[���ɑJ�ڂ�����
   * @return
   */
  bool MoveToNextFrame();

 private:
  //! �g���v���o�b�t�@���g�p����
  static constexpr u32 FRAME_COUNT = 3;

 private:
  ComPtr<ID3D12Device> device_;
  device::DXGIAdapter adapter_;
  //! �����_�[�^�[�Q�b�g�ƂȂ�E�B���h�E
  std::weak_ptr<window::Window> target_window_;
  //! �����_�[�^�[�Q�b�g�ƂȂ�X�N���[���̑傫��
  math::IntVector2 render_target_screen_size_;
  //! �R�}���h�L���[
  ComPtr<ID3D12CommandQueue> command_queue_;
  //! �X���b�v�`�F�C��
  ComPtr<IDXGISwapChain4> swap_chain_;
  //! �o�b�t�@�C���f�b�N�X
  i32 frame_index_;
  //! �����_�[�^�[�Q�b�g�̃��\�[�X
  std::array<buffer::RenderTarget, FRAME_COUNT> render_targets_;
  //! �����_�[�^�[�Q�b�g�̃q�[�v�T�C�Y
  i32 rtv_heap_size_;
  //! �R�}���h�A���P�[�^
  std::array<ComPtr<ID3D12CommandAllocator>, FRAME_COUNT> command_allocator_;
  //! �R�}���h�t�F���X
  ComPtr<ID3D12Fence> fence_;
  //! �R�}���h���X�g
  ComPtr<ID3D12GraphicsCommandList4> command_list_;
  //!< ���݂̃����_�[�^�[�Q�b�g�̏��
  D3D12_RESOURCE_STATES current_resource_state_;
  //! �t�F���X�l
  std::array<u64, FRAME_COUNT> fence_values_;
  //! �t�F���X�C�x���g
  Microsoft::WRL::Wrappers::Event fence_event_;
  //! �f�B�X�N���v�^�q�[�v�Ǘ�
  HeapManager heap_manager_;
};
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX12_DEVICE_H_
