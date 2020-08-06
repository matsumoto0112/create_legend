#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_H_

/**
 * @file render_target.h
 * @brief �����_�[�^�[�Q�b�g�N���X��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/device/directx_accessor.h"
#include "src/util/color_4.h"

namespace legend {
namespace directx {
namespace render_target {
/**
 * @class RenderTarget
 * @brief �����_�[�^�[�Q�b�g
 */
class RenderTarget {
 public:
  struct RenderTargetDesc {
    std::wstring name;
    u32 width;
    u32 height;
    DXGI_FORMAT format;
    util::Color4 clear_color;
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  RenderTarget();
  /**
   * @brief �f�X�g���N�^
   */
  ~RenderTarget();
  /**
   * @brief ������
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor, const RenderTargetDesc& desc);
  bool InitFromBuffer(device::IDirectXAccessor& accessor,
                      ComPtr<ID3D12Resource> buffer,
                      const util::Color4& clear_color,
                      const std::wstring& name);

  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_state);
  void ClearRenderTarget(device::CommandList& command_list) const;
  ///**
  // * @brief �p�C�v���C���X�e�[�g��RTV������������
  // * @param pipeline �������ޑΏ�
  // */
  // void WriteInfoToPipelineState(shader::GraphicsPipelineState* pipeline)
  // const;

 public:
  /**
   * @brief ���\�[�X���擾����
   */
  inline ID3D12Resource* GetResource() const { return resource_.GetResource(); }
  /**
   * @brief �n���h�����擾����
   */
  inline descriptor_heap::DescriptorHandle GetHandle() const {
    return rtv_handle_;
  }
  /**
   * @brief �t�H�[�}�b�g���擾����
   */
  inline DXGI_FORMAT GetFormat() const { return format_; }
  /**
   * @brief �r���[�|�[�g���擾����
   */
  inline D3D12_VIEWPORT GetViewport() const { return viewport_; }
  /**
   * @brief �V�U�[��`���擾����
   */
  inline D3D12_RECT GetScissorRect() const { return scissor_rect_; }

 private:
  //! ���\�[�X
  buffer::CommittedResource resource_;
  //! �����_�[�^�[�Q�b�g�n���h��
  descriptor_heap::DescriptorHandle rtv_handle_;
  //! �o�b�t�@�̃N���A�F
  util::Color4 clear_color_;
  //! �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g
  DXGI_FORMAT format_;
  //! �r���[�|�[�g
  D3D12_VIEWPORT viewport_;
  //! �V�U�[��`
  D3D12_RECT scissor_rect_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_H_
