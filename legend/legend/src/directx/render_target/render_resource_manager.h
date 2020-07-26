#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_

/**
 * @file render_resource_manager.h
 * @brief �����_�[�^�[�Q�b�g���\�[�X�Ǘ��N���X��`
 */

#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/device/swap_chain.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/render_target/depth_stencil.h"
#include "src/directx/render_target/depth_stencil_target_id.h"
#include "src/directx/render_target/multi_render_target_texture.h"
#include "src/directx/render_target/render_target_id.h"
#include "src/window/window.h"

namespace legend {
namespace directx {
namespace render_target {

/**
 * @class RenderResourceManager
 * @brief �����_�[�^�[�Q�b�g���\�[�X�Ǘ��N���X
 */
class RenderResourceManager {
 private:
  using RenderTargetMap =
      std::unordered_map<RenderTargetID, MultiRenderTargetTexture>;
  using DepthStencilTargetMap =
      std::unordered_map<DepthStencilTargetID, DepthStencil>;

 public:
  using Info = MultiRenderTargetTexture::Info;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  RenderResourceManager();
  /**
   * @brief �f�X�g���N�^
   */
  ~RenderResourceManager();
  /**
   * @brief ������
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param adapter �A�_�v�^�[
   * @param target_window �`��Ώۂ̃E�B���h�E
   * @param frame_count �o�b�N�o�b�t�@��
   * @param format �o�b�N�o�b�t�@�̃t�H�[�}�b�g
   * @param command_queue �X���b�v�`�F�C��
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, device::DXGIAdapter& adapter,
            window::Window& target_window, u32 frame_count, DXGI_FORMAT format,
            ID3D12CommandQueue* command_queue);
  /**
   * @brief �����_�[�^�[�Q�b�g���쐬����
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param unique_id �����_�[�^�[�Q�b�gID
   * @param format �t�H�[�}�b�g
   * @param width ��
   * @param height ����
   * @param clear_color �N���A�F
   * @param name �����_�[�^�[�Q�b�g��
   * @return �쐬�ɐ���������true��Ԃ�
   */
  bool CreateRenderTarget(IDirectXAccessor& accessor, RenderTargetID unique_id,
                          DXGI_FORMAT format, u32 width, u32 height,
                          const util::Color4& clear_color,
                          const std::wstring& name);
  /**
   * @brief �}���`�����_�[�^�[�Q�b�g���쐬����
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param unique_id �����_�[�^�[�Q�b�gID
   * @param info ���ꂼ��̃����_�[�^�[�Q�b�g�̏��
   * @return �쐬�ɐ���������true��Ԃ�
   */
  bool CreateRenderTargets(IDirectXAccessor& accessor, RenderTargetID unique_id,
                           const std::vector<Info>& info);
  /**
   * @brief �f�v�X�E�X�e���V�����쐬����
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param unique_id �f�v�X�E�X�e���V��ID
   * @param format �t�H�[�}�b�g
   * @param width ��
   * @param height ����
   * @param depth_clear_value �f�v�X�l�̃N���A�l
   * @param stencil_clear_value �X�e���V���l�̃N���A�lk
   * @param name �f�v�X�E�X�e���V����
   * @return �쐬�ɐ���������true��Ԃ�
   */
  bool CreateDepthStencil(IDirectXAccessor& accessor,
                          DepthStencilTargetID unique_id, DXGI_FORMAT format,
                          u32 width, u32 height, float depth_clear_value,
                          u8 stencil_clear_value, const std::wstring& name);
  /**
   * @brief �ΏۂƂȂ郌���_�[�^�[�Q�b�g��؂�ւ���
   �R�}���h���X�g�ɂ̓Z�b�g���Ȃ�
   */
  void SetRenderTargetID(RenderTargetID unique_id);
  /**
   * @brief �ΏۂƂȂ�f�v�X�E�X�e���V����؂�ւ���
    �R�}���h���X�g�ɂ̓Z�b�g���Ȃ�
  */
  void SetDepthStencilTargetID(DepthStencilTargetID unique_id);
  /**
   * @brief �����_�[�^�[�Q�b�g�̏����p�C�v���C���ɏ�������
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param unique_id �����_�[�^�[�Q�b�gID
   * @param pipeline �������ޑΏۂ̃p�C�v���C��
   */
  void WriteRenderTargetInfoToPipeline(IDirectXAccessor& accessor,
                                       RenderTargetID unique_id,
                                       shader::GraphicsPipelineState* pipeline);
  /**
   * @brief �f�v�X�E�X�e���V���̏����p�C�v���C���ɏ�������
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param unique_id �f�v�X�E�X�e���V��ID
   * @param pipeline �������ޑΏۂ̃p�C�v���C��
   */
  void WriteDepthStencilTargetInfoToPipeline(
      IDirectXAccessor& accessor, DepthStencilTargetID unique_id,
      shader::GraphicsPipelineState* pipeline);
  /**
   * @brief �����_�[�^�[�Q�b�g���V�F�[�_�[���\�[�X�Ƃ��ė��p����
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param unique_id �����_�[�^�[�Q�b�gID
   * @param render_target_number
   * �g�p�����������_�[�^�[�Q�b�g�̔ԍ�
   */
  void UseRenderTargetToShaderResource(IDirectXAccessor& accessor,
                                       RenderTargetID unique_id,
                                       u32 render_target_number = 0);
  /**
   * @brief ���ݑΏۂƂȂ��Ă��郌���_�[�^�[�Q�b�g���N���A����
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   */
  void ClearCurrentRenderTarget(IDirectXAccessor& accessor);
  /**
   * @brief ���ݑΏۂƂȂ��Ă���f�v�X�E�X�e���V�����N���A����
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   */
  void ClearCurrentDepthStencilTarget(IDirectXAccessor& accessor);
  /**
   * @brief ���ݑΏۂƂȂ��Ă��郌���_�[�^�[�Q�b�g���R�}���h���X�g�ɃZ�b�g����
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   */
  void SetRenderTargetsToCommandList(IDirectXAccessor& accessor);
  /**
   * @brief �`����I������
   * @param accessor
   */
  void DrawEnd(IDirectXAccessor& accessor);
  /**
   * @brief ������ID�����łɓo�^����Ă��邩
   * @param unique_id ���肷��ID
   * @return �o�^����Ă�����true��Ԃ�
   */
  bool IsRegisteredRenderTargetID(RenderTargetID unique_id) const;
  /**
   * @brief ������ID�����łɓo�^����Ă��邩
   * @param unique_id ���肷��ID
   * @return �o�^����Ă�����true��Ԃ�
   */
  bool IsRegisterdDepthStencilTargetID(DepthStencilTargetID unique_id) const;
  /**
   * @brief �o�b�N�o�b�t�@����ʂɕ\������
   * @return �\���ɐ���������true��Ԃ�
   */
  bool Present();
  /**
   * @brief ���݃t���[���̃C���f�b�N�X���X�V����
   */
  void UpdateCurrentFrameIndex();
  /**
   * @brief ���݃t���[���̃C���f�b�N�X���擾����
   */
  u32 GetCurrentFrameIndex() const;

 private:
  //! �X���b�v�`�F�C��
  device::SwapChain swap_chain_;
  //! �����_�[�^�[�Q�b�g�}�b�v
  RenderTargetMap render_targets_;
  //! ���ݑΏۂƂȂ��Ă��郌���_�[�^�[�Q�b�gID
  RenderTargetID current_render_target_id_;
  //! �f�v�X�E�X�e���V���}�b�v
  DepthStencilTargetMap depth_stencil_targets_;
  //! ���ݑΏۂƂȂ��Ă���f�v�X�E�X�e���V��ID
  DepthStencilTargetID current_depth_stencil_target_id_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_
