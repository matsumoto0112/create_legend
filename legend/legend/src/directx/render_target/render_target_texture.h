#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_TEXTURE_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_TEXTURE_H_

/**
 * @file render_target_texture.h
 * @brief �e�N�X�`���g�p�\�ȃ����_�[�^�[�Q�b�g�N���X��`
 */

#include "src/directx/render_target/render_target.h"

namespace legend {
namespace directx {
namespace render_target {
/**
 * @class RenderTargetTexture
 * @brief �e�N�X�`���g�p�\�ȃ����_�[�^�[�Q�b�g
 */
class RenderTargetTexture {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  RenderTargetTexture();
  /**
   * @brief �f�X�g���N�^
   */
  ~RenderTargetTexture();
  /**
   * @brief ������
   * @param accessor DirectX12�A�N�Z�T
   * @param register_num �V�F�[�_�[�̃��W�X�^�[�ԍ�
   * @param format �e�N�X�`���t�H�[�}�b�g
   * @param width �e�N�X�`����
   * @param height �e�N�X�`������
   * @param clear_color �����_�[�^�[�Q�b�g�̃N���A�F
   * @param name ���\�[�X��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, u32 register_num, DXGI_FORMAT format,
            u32 width, u32 height, const util::Color4& clear_color,
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
   * @brief �e�N�X�`����SRV�Ƃ��ăO���[�o���q�[�v�ɃZ�b�g����
   * @param accessor DirextX12�A�N�Z�T
   */
  void SetToGlobalHeap(IDirectXAccessor& accessor) const;

 public:
  inline const RenderTarget& GetRenderTarget() const { return render_target_; }

 private:
  //! �����_�[�^�[�Q�b�g
  RenderTarget render_target_;
  //! SRV�n���h��
  DescriptorHandle srv_handle_;
  //! �V�F�[�_�[�̃��W�X�^�[�ԍ�
  u32 register_num_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_TEXTURE_H_
