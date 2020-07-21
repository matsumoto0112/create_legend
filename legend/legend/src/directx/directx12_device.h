#ifndef LEGEND_DIRECTX_DIRECTX12_DEVICE_H_
#define LEGEND_DIRECTX_DIRECTX12_DEVICE_H_

/**
 * @file directx12_device.h
 * @brief DirectX12�f�o�C�X��`
 */

#include <wrl/wrappers/corewrappers.h>

#include "src/directx/device/command_list.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/device/swap_chain.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/descriptor_heap/heap_manager.h"
#include "src/directx/render_target/render_resource_manager.h"
#include "src/directx/shader/root_signature.h"
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
  bool Init(std::weak_ptr<window::Window> target_window);
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

  /**
   * @brief �o�b�N�o�b�t�@�̃����_�[�^�[�Q�b�g���Z�b�g����
   */
  void SetBackBuffer();

  /**
   * @brief GPU�̏�����ҋ@����
   */
  void WaitForGPU() noexcept;
  /**
   * @brief �f�B�X�N���v�^�n���h�����擾����
   * @param heap_type �擾����f�B�X�N���v�^�q�[�v�̎��
   */
  virtual DescriptorHandle GetHandle(DescriptorHeapType heap_type) override;
  /**
   * @brief �O���[�o���q�[�v�Ƀf�B�X�N���v�^�n���h�����Z�b�g����
   * @param register_num �Z�b�g����n���h���̃V�F�[�_�ɂ����郌�W�X�^�[�ԍ�
   * @param resource_type ���\�[�X�̎��
   * @param handle �Z�b�g����n���h��
   */
  virtual void SetToGlobalHeap(u32 register_num, ResourceType resource_type,
                               const DescriptorHandle& handle) override;

  virtual DescriptorHandle GetBackBufferHandle() const override;
  virtual void ClearBackBufferTarget(IDirectXAccessor& accessor) override;
  virtual DXGI_FORMAT GetBackBufferFormat() const;
  virtual void SetBackBuffer(IDirectXAccessor& accessor) override;
 public:
  virtual inline ID3D12Device* GetDevice() const override {
    return device_.Get();
  }
  virtual inline ID3D12GraphicsCommandList4* GetCommandList() const override {
    return command_lists_[frame_index_].GetCommandList();
  }
  const inline render_target::RenderTarget& GetRenderTarget() const {
    return swap_chain_.GetRenderTarget();
  }
  inline render_target::RenderResourceManager& GetRenderResourceManager() {
    return render_resource_manager_;
  }
  /**
   * @brief �f�B�X�N���v�^�q�[�v�Ǘ��҂��擾����
   */
  inline HeapManager& GetHeapManager() { return heap_manager_; }
  /**
   * @brief �f�t�H���g�̃��[�g�V�O�l�`�����擾����
   */
  inline std::shared_ptr<shader::RootSignature> GetDefaultRootSignature()
      const {
    return default_root_signature_;
  }

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
  //! �f�o�C�X
  ComPtr<ID3D12Device> device_;
  //! �A�_�v�^�[
  device::DXGIAdapter adapter_;
  //! �X���b�v�`�F�C��
  device::SwapChain swap_chain_;
  //! �����_�[�^�[�Q�b�g�ƂȂ�E�B���h�E
  std::weak_ptr<window::Window> target_window_;
  //! �����_�[�^�[�Q�b�g�ƂȂ�X�N���[���̑傫��
  math::IntVector2 render_target_screen_size_;
  //! �R�}���h�L���[
  ComPtr<ID3D12CommandQueue> command_queue_;
  //! �o�b�t�@�C���f�b�N�X
  i32 frame_index_;
  //! �R�}���h�A���P�[�^
  std::array<device::CommandList, FRAME_COUNT> command_lists_;
  //! �R�}���h�t�F���X
  ComPtr<ID3D12Fence> fence_;
  //!< ���݂̃����_�[�^�[�Q�b�g�̏��
  D3D12_RESOURCE_STATES current_resource_state_;
  //! �t�F���X�l
  std::array<u64, FRAME_COUNT> fence_values_;
  //! �t�F���X�C�x���g
  Microsoft::WRL::Wrappers::Event fence_event_;
  //! �f�B�X�N���v�^�q�[�v�Ǘ�
  HeapManager heap_manager_;
  //! �f�t�H���g�̃��[�g�V�O�l�`��
  std::shared_ptr<shader::RootSignature> default_root_signature_;
  render_target::RenderResourceManager render_resource_manager_;
};
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX12_DEVICE_H_
