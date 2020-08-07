#ifndef LEGEND_DIRECTX_RENDER_TARGET_MULTI_RENDER_TARGET_TEXTURE_H_
#define LEGEND_DIRECTX_RENDER_TARGET_MULTI_RENDER_TARGET_TEXTURE_H_

/**
 * @file multi_render_target_texture.h
 * @brief �e�N�X�`���g�p�\�ȃ����_�[�^�[�Q�b�g�N���X��`
 */

#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/device/command_list.h"
#include "src/directx/render_target/render_target.h"

namespace legend {
namespace directx {
namespace render_target {

/**
 * @class MultiRenderTargetTexture
 * @brief �e�N�X�`���g�p�\�ȃ����_�[�^�[�Q�b�g
 */
class MultiRenderTargetTexture {
 private:
  /**
   * @struct RenderTargetTexture
   * @brief �����_�[�^�[�Q�b�g�e�N�X�`���\����
   */
  struct RenderTargetTexture {
    //! �V�F�[�_�[�̃��W�X�^�[�ԍ�
    u32 register_num;
    //! �����_�[�^�[�Q�b�g
    RenderTarget render_target;
    //! SRV�n���h��
    descriptor_heap::DescriptorHandle srv_handle;
  };

 public:
  /**
   * @struct Info
   * @brief �����_�[�^�[�Q�b�g�̏��
   */
  struct Info {
    //! �V�F�[�_�[�̃��W�X�^�[�ԍ�
    u32 register_num;
    //! �t�H�[�}�b�g
    DXGI_FORMAT format;
    //! ��
    u32 width;
    //! ����
    u32 height;
    //! �N���A�F
    util::Color4 clear_color;
    //! ���\�[�X��
    std::wstring name;
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  MultiRenderTargetTexture();
  /**
   * @brief �f�X�g���N�^
   */
  ~MultiRenderTargetTexture();
  /**
   * @brief �V���O�������_�[�^�[�Q�b�g�Ƃ��ď���������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param srv_local_heap_id �V�F�[�_�[���\�[�X�̃��[�J���q�[�vID
   * @param info �����_�[�^�[�Q�b�g�̏��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor,
            descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
            const Info& info);
  /**
   * @brief �}���`�����_�[�^�[�Q�b�g�Ƃ��ď���������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param srv_local_heap_id �V�F�[�_�[���\�[�X�̃��[�J���q�[�vID
   * @param infos �e�����_�[�^�[�Q�b�g�̏��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor,
            descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
            const std::vector<Info>& infos);
  /**
   * @brief �o�b�t�@���珉��������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param buffer �o�b�t�@
   * @param clear_color �����_�[�^�[�Q�b�g�̃N���A�F
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   * @details ��Ƀo�b�N�o�b�t�@�̃����_�[�^�[�Q�b�g�Ɏg�p����
   �V�F�[�_�[���\�[�X�Ƃ��Ă͎g�p���Ȃ��ݒ�ɂ���
   */
  bool InitFromBuffer(device::IDirectXAccessor& accessor,
                      ComPtr<ID3D12Resource> buffer,
                      const util::Color4& clear_color,
                      const std::wstring& name);
  /**
   * @brief �����_�[�^�[�Q�b�g�̐F���N���A����
   * @param command_list �R�}���h���X�g
   * @details �����_�[�^�[�Q�b�g�ɃZ�b�g����Ă��Ȃ��Ƃ��͖���
   */
  void ClearRenderTarget(device::CommandList& command_list) const;
  /**
   * @brief �V�F�[�_�[���\�[�X�r���[�Ƃ��Ďg�p����
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param render_target_number �g�p���郌���_�[�^�[�Q�b�g�ԍ�
   * @details render_target_number�͉��Ԗڂ̃e�N�X�`�����g�p���邩���w�肷��
   ���O��Transition�ŃV�F�[�_�[���\�[�X�Ƃ��Ĉ�����悤�ɂ��Ă����K�v������
   */
  void UseAsSRV(device::IDirectXAccessor& accessor, u32 render_target_number);
  /**
   * @brief �n���h�����擾����
   */
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> GetRTVHandles() const {
    return rtv_handles_;
  }
  /**
   * @brief �r���[�|�[�g���Z�b�g����
   * @param command_list �R�}���h���X�g
   */
  void SetViewport(device::CommandList& command_list) const;
  /**
   * @brief �V�U�[��`���Z�b�g����
   * @param command_list �R�}���h���X�g
   */
  void SetScissorRect(device::CommandList& command_list) const;
  /**
   * @brief ��Ԃ�J�ڂ�����
   * @param command_list �R�}���h���X�g
   * @param next_state ���̏��
   */
  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_state);

 private:
  //! �����_�[�^�[�Q�b�g��
  u32 render_target_num_;
  //! �r���[�|�[�g
  std::vector<D3D12_VIEWPORT> viewports_;
  //! �V�U�[��`
  std::vector<D3D12_RECT> scissor_rects_;
  //! �����_�[�^�[�Q�b�g
  std::vector<RenderTargetTexture> render_targets_;
  //! �n���h��
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtv_handles_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_MULTI_RENDER_TARGET_TEXTURE_H_
