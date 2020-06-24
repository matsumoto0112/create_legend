#ifndef LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_
#define LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_

/**
 * @file pipeline_state.h
 * @brief パイプラインステート定義
 */

#include "src/directx/directx12_device.h"
#include "src/directx/shader/pixel_shader.h"
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
  bool Init(DirectX12Device& device);
  /**
   * @brief 頂点シェーダーをセットする
   */
  void SetVertexShader(std::shared_ptr<VertexShader> vertex_shader);
  /**
   * @brief ピクセルシェーダーをセットする
   */
  void SetPixelShader(std::shared_ptr<PixelShader> pixel_shader);
  /**
   * @brief パイプラインステートを作成する
   * @param device DirectX12デバイス
   */
  bool CreatePipelineState(DirectX12Device& device);
  /**
   * @brief コマンドリストにセットする
   * @param device DirectX12デバイス
   */
  void SetGraphicsCommandList(DirectX12Device& device);

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
  ComPtr<ID3D12RootSignature> root_signature_;

  ComPtr<ID3D12DescriptorHeap> heap_;
  struct Color {
    float color[4];
  } color_;
  ComPtr<ID3D12Resource> constant_buffer_;
  
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_
