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
  /**
   * @brief �f�v�X�E�X�e���V���f�X�N
   */
  struct DepthStencilDesc {
    //! ���\�[�X��
    std::wstring name;
    //! �t�H�[�}�b�g
    DXGI_FORMAT format;
    //! ��
    u32 width;
    //! ����
    u32 height;
    //! �f�v�X�l
    float depth_value;
    //! �X�e���V���l
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
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param desc �f�v�X�E�X�e���V���f�X�N
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor, const DepthStencilDesc& desc);
  /**
   * @brief �f�v�X�E�X�e���V���l�̃N���A
   * @param command_list �R�}���h���X�g
   */
  void ClearDepthStencil(device::CommandList& command_list) const;
  /**
   * @brief ��Ԃ�J�ڂ�����
   * @param command_list �R�}���h���X�g
   * @param next_states ���̏��
   */
  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_states);

  void UseAsSRV(device::IDirectXAccessor& accessor,
                device::CommandList& command_list, u32 register_num);
  /**
   * @brief �f�B�X�N���v�^�n���h�����擾����
   */
  descriptor_heap::DescriptorHandle GetHandle() const { return handle_; }

  DXGI_FORMAT GetFormat() const { return format_; }

  D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() const;

 private:
  //! ���\�[�X
  buffer::CommittedResource resource_;
  //! �n���h��
  descriptor_heap::DescriptorHandle handle_;
  descriptor_heap::DescriptorHandle srv_handle_;
  //! �t�H�[�}�b�g
  DXGI_FORMAT format_;
  //! �f�v�X�l
  float depth_value_;
  //! �X�e���V���l
  u8 stencil_value_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_
