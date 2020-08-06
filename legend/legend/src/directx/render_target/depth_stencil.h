#ifndef LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_
#define LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_

/**
 * @file depth_stencil.h
 * @brief �f�v�X�E�X�e���V���N���X��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace render_target {
/**
 * @class DepthStencil
 * @brief �f�v�X�E�X�e���V���N���X
 */
class DepthStencil {
 public:
  struct DepthStencilDesc {
    std::wstring name;
    DXGI_FORMAT format;
    u32 width;
    u32 height;
    float depth_value;
    u8 stencil_value;
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  DepthStencil();
  /**
   * @brief �f�X�g���N�^
   */
  ~DepthStencil();
  /**
   * @brief ������
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor, const DepthStencilDesc& desc);
  /**
   * @brief �f�v�X�E�X�e���V���l�̃N���A
   */
  void ClearDepthStencil(device::CommandList& command_list) const;
  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_states);
  descriptor_heap::DescriptorHandle GetHandle() const { return handle_; }

 private:
  //! ���\�[�X
  buffer::CommittedResource resource_;
  //! �n���h��
  descriptor_heap::DescriptorHandle handle_;
  //! �t�H�[�}�b�g
  DXGI_FORMAT format_;
  //! �N���A�l
  float depth_value_;
  u8 stencil_value_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_
