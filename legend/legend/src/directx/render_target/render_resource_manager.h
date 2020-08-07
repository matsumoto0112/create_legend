#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_

/**
 * @file render_resource_manager.h
 * @brief �����_�[�^�[�Q�b�g���\�[�X�Ǘ��N���X��`
 */

#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/device/swap_chain.h"
#include "src/directx/render_target/depth_stencil.h"
#include "src/directx/render_target/multi_render_target_texture.h"
#include "src/directx/render_target/render_target_id.h"

namespace legend {
namespace directx {
namespace render_target {

/**
 * @class RenderResourceManager
 * @brief �����_�[�^�[�Q�b�g���\�[�X�Ǘ��N���X
 */
class RenderResourceManager {
 private:
  /**
   * @brief �����t���[�����̃^�[�Q�b�g�Ǘ��N���X
   * @tparam T �Ǘ�����^�[�Q�b�g�̌^
   */
  template <class T>
  struct MultiframeTarget {
    /**
     * @brief �R���X�g���N�^
     * @param frame_count �t���[����
     * @param swap_chain �X���b�v�`�F�C��
     */
    inline MultiframeTarget(u32 frame_count, device::SwapChain& swap_chain)
        : swap_chain(swap_chain) {
      this->targets.resize(frame_count);
    }
    /**
     * @brief ���݃t���[���̃^�[�Q�b�g���擾����
     */
    inline T& Get() { return targets[swap_chain.GetCurrentBackBufferIndex()]; }
    /**
     * @brief ���݃t���[���̃^�[�Q�b�g���擾����
     */
    inline const T& Get() const {
      return targets[swap_chain.GetCurrentBackBufferIndex()];
    }
    /**
     * @brief index����^�[�Q�b�g���擾����
     */
    inline T& Get(u32 index) { return targets[index]; }

   private:
    //! �X���b�v�`�F�C��
    device::SwapChain& swap_chain;
    //! �t���[�������̃^�[�Q�b�g
    std::vector<T> targets;
  };

  //! �}���`�����_�[�^�[�Q�b�g
  using MultiFrameMultiRenderTargetTexture =
      MultiframeTarget<MultiRenderTargetTexture>;
  //! �f�v�X�E�X�e���V��
  using MultiFrameDepthStencil = MultiframeTarget<DepthStencil>;
  using RenderTargetMap =
      std::unordered_map<RenderTargetID, MultiFrameMultiRenderTargetTexture>;
  using DepthStencilTargetMap =
      std::unordered_map<DepthStencilTargetID, MultiFrameDepthStencil>;

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
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param adapter �A�_�v�^�[
   * @param frame_count �t���[����
   * @param width ��
   * @param height ����
   * @param format �t�H�[�}�b�g
   * @param hwnd �E�B���h�E�n���h��
   * @param command_queue �R�}���h�L���[
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor, device::DXGIAdapter& adapter,
            u32 frame_count, u32 width, u32 height, DXGI_FORMAT format,
            HWND hwnd, ID3D12CommandQueue* command_queue);
  /**
   * @brief �����_�[�^�[�Q�b�g���Z�b�g����
   * @param command_list �R�}���h���X�g
   * @param render_target_id �����_�[�^�[�Q�b�gID
   * @param clear_render_target �����_�[�^�[�Q�b�g���N���A���邩
   * @param depth_stencil_target_id �f�v�X�E�X�e���V��ID
   * @param clear_depth_stencil_target �f�v�X�E�X�e���V�����N���A���邩
   */
  void SetRenderTargets(device::CommandList& command_list,
                        RenderTargetID render_target_id,
                        bool clear_render_target,
                        DepthStencilTargetID depth_stencil_target_id,
                        bool clear_depth_stencil_target);
  /**
   * @brief �o�b�N�o�b�t�@�̃C���f�b�N�X���擾����
   */
  u32 GetCurrentBackBufferIndex() const;
  /**
   * @brief �`��I������
   * @param command_list �R�}���h���X�g
   */
  void DrawEnd(device::CommandList& command_list);
  /**
   * @brief �`����e��\������
   * @return ����������true��Ԃ�
   */
  bool Present();
  /**
   * @brief �����_�[�^�[�Q�b�g��ǉ�����
   * @param id �����_�[�^�[�Q�b�gID
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param infos �����_�[�^�[�Q�b�g���
   * @return �ǉ��ɐ���������true��Ԃ�
   */
  bool AddRenderTarget(
      RenderTargetID id, device::IDirectXAccessor& accessor,
      const std::vector<MultiRenderTargetTexture::Info>& infos);
  /**
   * @brief �f�v�X�E�X�e���V����ǉ�����
   * @param id �����_�[�^�[�Q�b�gID
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param desc �f�v�X�E�X�e���V���f�X�N
   * @return �ǉ��ɐ���������true��Ԃ�
   */
  bool AddDepthStencil(DepthStencilTargetID id,
                       device::IDirectXAccessor& accessor,
                       const DepthStencil::DepthStencilDesc& desc);
  /**
   * @brief �V�F�[�_�[���\�[�X�Ƃ��Ďg�p����
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param id �����_�[�^�[�Q�b�gID
   * @param render_target_index �g�p����}���`�����_�[�^�[�Q�b�g�̃C���f�b�N�X
   */
  void UseAsSRV(device::IDirectXAccessor& accessor, RenderTargetID id,
                u32 render_target_index);

 private:
  //! �t���[����
  u32 frame_count_;
  //! �X���b�v�`�F�C��
  device::SwapChain swap_chain_;
  //! �����_�[�^�[�Q�b�g�}�b�v
  RenderTargetMap render_targets_;
  //! �f�v�X�E�X�e���V���}�b�v
  DepthStencilTargetMap depth_stencil_targets_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_
