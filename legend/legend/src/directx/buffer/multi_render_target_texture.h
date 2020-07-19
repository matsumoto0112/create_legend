#ifndef LEGEND_DIRECTX_BUFFER_MULTI_RENDER_TARGET_TEXTURE_H_
#define LEGEND_DIRECTX_BUFFER_MULTI_RENDER_TARGET_TEXTURE_H_

/**
 * @file multi_render_target_texture.h
 * @brief �e�N�X�`�����p�\�ȃ}���`�����_�[�^�[�Q�b�g�N���X��`
 */

#include "src/directx/buffer/render_target_texture.h"
#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {
class MultiRenderTargetTexture {
 public:
  struct RenderTargetCreateInfo {
    u32 register_num;
    util::Color4 clear_color;
  };

 public:
  MultiRenderTargetTexture();
  ~MultiRenderTargetTexture();
  bool Init(IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width,
            u32 height, const std::vector<RenderTargetCreateInfo>& infos,
            const std::wstring& name_base);
  void WriteInfoToPipelineStateDesc(
      D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc) const;
  /**
   * @brief �����_�[�^�[�Q�b�g�ɃZ�b�g����
   * @param accessor DirextX12�A�N�Z�T
   */
  void SetRenderTarget(IDirectXAccessor& accessor);
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
  void SetToGlobalHeap(IDirectXAccessor& accessor) const;

 private:
  //! ���\�[�X
  std::vector<CommittedResource> render_target_resources_;
  std::vector<RenderTargetCreateInfo> infos_;
  std::vector<DescriptorHandle> handles_;
  std::vector<DescriptorHandle> srv_handles_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_MULTI_RENDER_TARGET_TEXTURE_H_
