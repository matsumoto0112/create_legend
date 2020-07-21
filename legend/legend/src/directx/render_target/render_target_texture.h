#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_TEXTURE_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_TEXTURE_H_

/**
 * @file render_target_texture.h
 * @brief �e�N�X�`���g�p�\�ȃ����_�[�^�[�Q�b�g�N���X��`
 */

#include "src/directx/render_target/render_target.h"
#include "src/directx/shader/graphics_pipeline_state.h"

namespace legend {
namespace directx {
namespace render_target {
/**
 * @class RenderTargetTexture
 * @brief �e�N�X�`���g�p�\�ȃ����_�[�^�[�Q�b�g
 */
class MultiRenderTargetTexture {
  struct RenderTargetTexture {
    u32 register_num;
    RenderTarget render_target;
    DescriptorHandle srv_handle;
  };

 public:
  struct Info {
    u32 register_num;
    DXGI_FORMAT format;
    u32 width;
    u32 height;
    util::Color4 clear_color;
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
   * @brief ������
   * @param accessor DirectX12�A�N�Z�T
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, const Info& info);
  bool Init(IDirectXAccessor& accessor, const std::vector<Info>& infos);
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
  void WriteInfoToPipelineDesc(shader::GraphicsPipelineState& pipeline);

  void PrepareToUseRenderTarget(IDirectXAccessor& accessor);
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> GetRTVHandles() const;

 private:
  std::vector<RenderTargetTexture> render_targets_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_TEXTURE_H_
