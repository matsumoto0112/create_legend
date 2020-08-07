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
  struct MultiFrameMultiRenderTargetTexture {
    std::vector<MultiRenderTargetTexture> mrt;
    device::SwapChain& swap_chain;
    MultiFrameMultiRenderTargetTexture(u32 frame_count,
                                       device::SwapChain& swap_chain)
        : swap_chain(swap_chain) {
      this->mrt.resize(frame_count);
    }

    MultiRenderTargetTexture& Get() {
      return mrt[swap_chain.GetCurrentBackBufferIndex()];
    }
    const MultiRenderTargetTexture& Get() const {
      return mrt[swap_chain.GetCurrentBackBufferIndex()];
    }
  };

  struct MultiFrameDepthStencil {
    std::vector<DepthStencil> ds;
    device::SwapChain& swap_chain;

    MultiFrameDepthStencil(u32 frame_count, device::SwapChain& swap_chain)
        : swap_chain(swap_chain) {
      this->ds.resize(frame_count);
    }

    DepthStencil& Get() { return ds[swap_chain.GetCurrentBackBufferIndex()]; }
    const DepthStencil& Get() const {
      return ds[swap_chain.GetCurrentBackBufferIndex()];
    }
  };

 private:
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
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param adapter �A�_�v�^�[
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor, device::DXGIAdapter& adapter,
            u32 frame_count, u32 width, u32 height, DXGI_FORMAT format,
            HWND hwnd, ID3D12CommandQueue* command_queue);

  void ClearCurrentRenderTarget(device::CommandList& command_list) const;
  void ClearCurrentDepthStencil(device::CommandList& command_list) const;

  void SetRenderTargets(device::CommandList& command_list);
  u32 GetCurrentBackBufferIndex() const;

  void DrawEnd(device::CommandList& command_list);
  bool Present();

  bool AddDepthStencil(DepthStencilTargetID id,
                       device::IDirectXAccessor& accessor,
                       const DepthStencil::DepthStencilDesc& desc);

  void SetRenderTargetID(RenderTargetID id) { current_render_target_id_ = id; }
  void SetDepthStencilTargetID(DepthStencilTargetID id) {
    current_depth_stencil_target_id_ = id;
  }

 private:
  u32 frame_count_;
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
