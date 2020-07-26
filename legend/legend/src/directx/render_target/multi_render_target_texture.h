#ifndef LEGEND_DIRECTX_RENDER_TARGET_MULTI_RENDER_TARGET_TEXTURE_H_
#define LEGEND_DIRECTX_RENDER_TARGET_MULTI_RENDER_TARGET_TEXTURE_H_

/**
 * @file multi_render_target_texture.h
 * @brief �e�N�X�`���g�p�\�ȃ����_�[�^�[�Q�b�g�N���X��`
 */

#include "src/directx/render_target/render_target.h"
#include "src/directx/shader/graphics_pipeline_state.h"

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
   * @param accessor DirectX12�A�N�Z�T
   * @param info �����_�[�^�[�Q�b�g�̏��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor,
            descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
            const Info& info);
  /**
   * @brief �}���`�����_�[�^�[�Q�b�g�Ƃ��ď���������
   * @param accessor DirectX12�A�N�Z�T
   * @param info �e�����_�[�^�[�Q�b�g�̏��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor,
            descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
            const std::vector<Info>& infos);
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
   * @brief �e�N�X�`����SRV�Ƃ��ăO���[�o���q�[�v�ɃZ�b�g����
   * @param accessor DirextX12�A�N�Z�T
   */
  void SetToGlobalHeap(IDirectXAccessor& accessor,
                       u32 render_target_number) const;
  void WriteInfoToPipelineDesc(shader::GraphicsPipelineState* pipeline);

  void PrepareToUseRenderTarget(IDirectXAccessor& accessor);
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> GetRTVHandles() const;

  void SetViewport(IDirectXAccessor& accessor) const;
  void SetScissorRect(IDirectXAccessor& accessor) const;

 private:
  std::vector<D3D12_VIEWPORT> viewports_;
  std::vector<D3D12_RECT> scissor_rects_;
  std::vector<RenderTargetTexture> render_targets_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_MULTI_RENDER_TARGET_TEXTURE_H_
