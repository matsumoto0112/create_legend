#ifndef LEGEND_DIRECTX_DEVICE_DIRECTX_DEVICE_H_
#define LEGEND_DIRECTX_DEVICE_DIRECTX_DEVICE_H_

/**
 * @file directx_device.h
 * @brief DirectX�f�o�C�X�N���X��`
 */

#include "src/directx/descriptor_heap/heap_manager.h"
#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/device/swap_chain.h"
#include "src/directx/frame_resource.h"
#include "src/directx/render_target/depth_stencil.h"
#include "src/directx/render_target/render_resource_manager.h"
#include "src/directx/shader/root_signature.h"

namespace legend {
namespace directx {
namespace device {

/**
 * @class DirectXDevice
 * @brief DirectX�f�o�C�X�N���X
 */
class DirectXDevice : public IDirectXAccessor {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  DirectXDevice();
  /**
   * @brief �f�X�g���N�^
   */
  ~DirectXDevice();
  /**
   * @brief ������
   * @param width ��ʕ�
   * @param height ��ʍ���
   * @param hwnd �E�B���h�E�n���h��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(u32 width, u32 height, HWND hwnd);
  /**
   * @brief �`�揀��
   * @return ����������true��Ԃ�
   */
  bool Prepare();
  /**
   * @brief �`�揈��
   * @return ����������true��Ԃ�
   */
  bool Present();
  /**
   * @brief �f�o�C�X��j������
   */
  void Destroy();

 public:
  /**
   * @brief �f�o�C�X���擾����
   */
  ID3D12Device* GetDevice() const override { return device_.Get(); }
  /**
   * @brief �����_�[�^�[�Q�b�g�n���h�����擾����
   */
  descriptor_heap::DescriptorHandle GetRTVHandle() override;
  /**
   * @brief �f�v�X�E�X�e���V���n���h�����擾����
   */
  descriptor_heap::DescriptorHandle GetDSVHandle() override;
  /**
   * @brief �n���h�����O���[�o���ɓo�^����
   * @param register_num �V�F�[�_�[�̃��W�X�^�[�ԍ�
   * @param type ���\�[�X�̎��
   * @param handle �o�^����n���h��
   */
  void RegisterHandle(u32 register_num, shader::ResourceType type,
                      descriptor_heap::DescriptorHandle handle);
  /**
   * @brief ���[�J���n���h�����擾����
   * @param heap_id �g�p���郍�[�J���n���h����ID
   */
  descriptor_heap::DescriptorHandle GetLocalHandle(
      descriptor_heap::heap_parameter::LocalHeapID heap_id) override;
  /**
   * @brief �q�[�v�Ǘ��I�u�W�F�N�g���擾����
   */
  descriptor_heap::HeapManager& GetHeapManager() const {
    return *heap_manager_;
  }
  /**
   * @brief �R�}���h���X�g�����s����
   * @param command_lists �Z�b�g����R�}���h���X�g
   */
  void ExecuteCommandList(const std::vector<CommandList>& command_lists);
  /**
   * @brief �R�}���h�̎��s��ҋ@����
   */
  void WaitExecute();
  /**
   * @brief �`�惊�\�[�X�Ǘ��I�u�W�F�N�g���擾����
   */
  render_target::RenderResourceManager& GetRenderResourceManager() {
    return render_resource_manager_;
  }
  /**
   * @brief ���݂̃t���[�����\�[�X���擾����
   */
  FrameResource* GetCurrentFrameResource() const { return current_resource_; }
  /**
   * @brief �f�t�H���g�̃��[�g�V�O�l�`�����擾����
   */
  shader::RootSignature* GetDefaultRootSignature() const {
    return default_root_signature_.get();
  }
  /**
   * @brief ���݂̃t���[���C���f�b�N�X���擾����
   */
  u32 GetCurrentBackBufferIndex() const override;

 private:
  //! �A�_�v�^�[
  DXGIAdapter adapter_;
  //!�f�o�C�X
  ComPtr<ID3D12Device> device_;
  //! �R�}���h�L���[
  ComPtr<ID3D12CommandQueue> command_queue_;

  //! �t���[���C���f�b�N�X
  u32 frame_index_;
  //! �����p�t�F���X
  ComPtr<ID3D12Fence> fence_;
  //! �����l
  UINT64 fence_value_;

  //! �t���[�����\�[�X
  FrameResource resources_[defines::FRAME_COUNT];
  //! ���݂̃t���[�����\�[�X
  FrameResource* current_resource_;
  //! �q�[�v�Ǘ��I�u�W�F�N�g
  std::unique_ptr<descriptor_heap::HeapManager> heap_manager_;
  //! �`�惊�\�[�X�Ǘ��I�u�W�F�N�g
  render_target::RenderResourceManager render_resource_manager_;
  //! �f�t�H���g�̃��[�g�V�O�l�`��
  std::unique_ptr<shader::RootSignature> default_root_signature_;
  //! �f�o�C�X�t���O
  DeviceOptionFlags option_flags_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_DEVICE_DIRECTX_DEVICE_H_
