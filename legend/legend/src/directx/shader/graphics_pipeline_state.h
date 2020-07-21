#ifndef LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_
#define LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_

/**
 * @file pipeline_state.h
 * @brief パイプラインステート定義
 */

#include "src/directx/directx_accessor.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/root_signature.h"
#include "src/directx/shader/vertex_shader.h"

namespace legend {
namespace directx {
namespace shader {
/**
 * @brief パイプラインステートクラス
 */
class GraphicsPipelineState {
 public:
  /**
   * @brief コンストラクタ
   */
  GraphicsPipelineState();
  /**
   * @brief デストラクタ
   */
  ~GraphicsPipelineState();
  /**
   * @brief 初期化
   * @param device DirectX12デバイス
   */
  bool Init(IDirectXAccessor& device);
  /**
   * @brief ルートシグネチャをセットする
   */
  void SetRootSignature(std::shared_ptr<RootSignature> root_signature);
  /**
   * @brief 頂点シェーダーをセットする
   */
  void SetVertexShader(std::shared_ptr<VertexShader> vertex_shader);
  /**
   * @brief ピクセルシェーダーをセットする
   */
  void SetPixelShader(std::shared_ptr<PixelShader> pixel_shader);
  /**
   * @brief アルファブレンドデスクを設定する
   */
  void SetBlendDesc(const D3D12_RENDER_TARGET_BLEND_DESC& blend_desc,
                    u32 rtv_index);
  /**
   * @brief 描画対象のプリミティブ形状を設定する
   */
  void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY_TYPE topology_type);
  void SetRTVFormat(DXGI_FORMAT format, u32 rtv_index);
  void SetRenderTargetNum(u32 num);
  /**
   * @brief パイプラインステートを作成する
   * @param device DirectX12デバイス
   */
  bool CreatePipelineState(IDirectXAccessor& device);
  /**
   * @brief コマンドリストにセットする
   * @param device DirectX12デバイス
   */
  void SetGraphicsCommandList(IDirectXAccessor& device);

 protected:
  //! パイプラインステートデスク
  D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc_;
  //! 頂点シェーダー
  std::shared_ptr<VertexShader> vertex_shader_;
  //! ピクセルシェーダー
  std::shared_ptr<PixelShader> pixel_shader_;
  //! パイプラインステート
  ComPtr<ID3D12PipelineState> pipeline_state_;
  //! ルートシグネチャ
  std::shared_ptr<RootSignature> root_signature_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_
