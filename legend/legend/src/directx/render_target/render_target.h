#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_H_

/**
 * @file render_target.h
 * @brief �����_�[�^�[�Q�b�g�N���X��`
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/shader/graphics_pipeline_state.h"
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
   * @param accessor DirextX12�A�N�Z�T
   * @param format �t�H�[�}�b�g
   * @param width �^�[�Q�b�g��
   * @param height �^�[�Q�b�g����
   * @param clear_color �o�b�t�@�̃N���A�F
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width,
            u32 height, const util::Color4& clear_color,
            const std::wstring& name);
  /**
   * @brief �o�b�t�@���珉��������
   * @param accessor DirextX12�A�N�Z�T
   * @param buffer ���ƂƂȂ�o�b�t�@
   * @param clear_color �o�b�t�@�̃N���A�F
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   * @detials �o�b�N�o�b�t�@�Ɋ�{�I�Ɏg��
   */
  bool InitFromBuffer(IDirectXAccessor& accessor, ComPtr<ID3D12Resource> buffer,
                      const util::Color4& clear_color,
                      const std::wstring& name);
  /**
   * @brief �����_�[�^�[�Q�b�g�̐F���N���A����
   * @param accessor DirextX12�A�N�Z�T
   * @details �����_�[�^�[�Q�b�g�ɃZ�b�g����Ă��Ȃ��Ƃ��͖���
   */
  void ClearRenderTarget(IDirectXAccessor& accessor) const;
  /**
   * @brief �`��I��
   * @param accessor DirextX12�A�N�Z�T
   */
  void DrawEnd(IDirectXAccessor& accessor);
  /**
   * @brief ��Ԃ�J�ڂ�����
   * @param accessor DirextX12�A�N�Z�T
   * @param next_state ���̏��
   */
  void Transition(IDirectXAccessor& accessor, D3D12_RESOURCE_STATES next_state);
  /**
   * @brief �p�C�v���C���X�e�[�g�f�X�N��RTV������������
   * @param pipeline_state_desc �������ޑΏ�
   */
  void WriteInfoToPipelineStateDesc(
      shader::GraphicsPipelineState& pipeline) const;

 public:
  /**
   * @brief ���\�[�X���擾����
   */
  ID3D12Resource* GetResource() const { return resource_.GetResource(); }
  /**
   * @brief �n���h�����擾����
   */
  descriptor_heap::DescriptorHandle GetHandle() const { return rtv_handle_; }
  inline DXGI_FORMAT GetFormat() const { return format_; }

 private:
  //! ���\�[�X
  buffer::CommittedResource resource_;
  //! �����_�[�^�[�Q�b�g�n���h��
  descriptor_heap::DescriptorHandle rtv_handle_;
  //! �o�b�t�@�̃N���A�F
  util::Color4 clear_color_;
  //! �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g
  DXGI_FORMAT format_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_H_
