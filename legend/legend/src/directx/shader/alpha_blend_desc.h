#ifndef LEGEND_DIRECTX_SHADER_ALPHA_BLEND_DESC_H_
#define LEGEND_DIRECTX_SHADER_ALPHA_BLEND_DESC_H_

/**
 * @file alpha_blend_desc.h
 * @brief アルファブレンドデスク定義
 */

namespace legend {
namespace directx {
namespace shader {
namespace alpha_blend_desc {

//! デフォルトのブレンド設定
static D3D12_RENDER_TARGET_BLEND_DESC BLEND_DESC_DEFAULT = []() {
  D3D12_RENDER_TARGET_BLEND_DESC desc{};
  desc.BlendEnable = FALSE;
  desc.LogicOpEnable = FALSE;
  desc.SrcBlend = D3D12_BLEND::D3D12_BLEND_ONE;
  desc.DestBlend = D3D12_BLEND::D3D12_BLEND_ZERO;
  desc.BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
  desc.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
  desc.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ZERO;
  desc.BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
  desc.LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
  desc.RenderTargetWriteMask =
      D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;
  return desc;
}();

//! 加算合成
static D3D12_RENDER_TARGET_BLEND_DESC BLEND_DESC_ADD = []() {
  D3D12_RENDER_TARGET_BLEND_DESC desc;
  desc.BlendEnable = TRUE;
  desc.LogicOpEnable = FALSE;
  desc.SrcBlend = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
  desc.DestBlend = D3D12_BLEND::D3D12_BLEND_ONE;
  desc.BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
  desc.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
  desc.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ZERO;
  desc.BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
  desc.LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
  desc.RenderTargetWriteMask =
      D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;
  return desc;
}();

//線形合成
static D3D12_RENDER_TARGET_BLEND_DESC BLEND_DESC_ALIGNMENT = []() {
  D3D12_RENDER_TARGET_BLEND_DESC desc;
  desc.BlendEnable = TRUE;
  desc.LogicOpEnable = FALSE;
  desc.SrcBlend = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
  desc.DestBlend = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
  desc.BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
  desc.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
  desc.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ZERO;
  desc.BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
  desc.LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
  desc.RenderTargetWriteMask =
      D3D12_COLOR_WRITE_ENABLE::D3D12_COLOR_WRITE_ENABLE_ALL;
  return desc;
}();

}  // namespace alpha_blend_desc
}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_ALPHA_BLEND_DESC_H_
