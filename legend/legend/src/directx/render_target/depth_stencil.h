#ifndef LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_
#define LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_

/**
 * @file depth_stencil.h
 * @brief �f�v�X�E�X�e���V���N���X��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @class DepthStencil
 * @brief �f�v�X�E�X�e���V���N���X
 */
class DepthStencil {
 public:
  /**
   * @struct ClearValue
   * @brief �f�v�X�E�X�e���V���̃N���A�l
   */
  struct ClearValue {
    //! �[�x�l
    float depth;
    //! �X�e���V���l
    u8 stencil;

    /**
     * @brief �R���X�g���N�^
     * @param depth �[�x�l
     * @param stencil �X�e���V���l
     */
    ClearValue(float depth = 1.0f, u8 stencil = 0)
        : depth(depth), stencil(stencil) {}
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
   * @param accessor DirextX12�f�o�C�X�A�N�Z�T
   * @param format �t�H�[�}�b�g
   * @param width ��
   * @param height ����
   * @param clear_value �N���A�l
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width,
            u32 height, const ClearValue& clear_value,
            const std::wstring& name);
  /**
   * @brief �f�v�X�E�X�e���V���l�̃N���A
   * @param accessor DirextX12�f�o�C�X�A�N�Z�T
   */
  void ClearDepthStencil(IDirectXAccessor& accessor) const;
  /**
   * @brief �R�}���h���X�g�ɃZ�b�g���鏀��������
   * @param accessor DirextX12�f�o�C�X�A�N�Z�T
   * @details RTV�Z�b�g�Ɠ�����DSV���Z�b�g���邽�߁A���̃Z�b�g�O�ɌĂԕK�v������
   */
  void PrepareToSetCommandList(IDirectXAccessor& accessor);
  /**
   * @brief �p�C�v���C���X�e�[�g�f�X�N��RTV������������
   * @param pipeline_state_desc �������ޑΏ�
   */
  void WriteInfoToPipelineStateDesc(
      D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc) const;

  /**
   * @brief CPU�n���h�����擾����
   * @details RTV�̃Z�b�g���ɓ�����DSV���Z�b�g���邽�߁ARTV�����Ɍ��J
   */
  D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() const {
    return handle_.cpu_handle_;
  }

 private:
  //! ���\�[�X
  legend::directx::buffer::CommittedResource resource_;
  //! �n���h��
  legend::directx::DescriptorHandle handle_;
  //! �t�H�[�}�b�g
  DXGI_FORMAT format_;
  //! �N���A�l
  ClearValue clear_value_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif //! LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_
