#include "src/directx/shader/defined_static_sampler.h"

namespace legend {
namespace directx {
namespace shader {
namespace defined_static_sampler {

// Warpサンプラー
D3D12_STATIC_SAMPLER_DESC Warp(u32 register_id) {
  D3D12_STATIC_SAMPLER_DESC warp_sampler = {};
  warp_sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
  warp_sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  warp_sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  warp_sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  warp_sampler.MinLOD = 0;
  warp_sampler.MaxLOD = D3D12_FLOAT32_MAX;
  warp_sampler.MipLODBias = 0.0f;
  warp_sampler.MaxAnisotropy = 1;
  warp_sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
  warp_sampler.BorderColor =
      D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
  warp_sampler.ShaderRegister = register_id;
  warp_sampler.ShaderVisibility =
      D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;

  return warp_sampler;
}

// Clampサンプラー
D3D12_STATIC_SAMPLER_DESC Clamp(u32 register_id) {
  D3D12_STATIC_SAMPLER_DESC clamp_sampler = {};
  clamp_sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
  clamp_sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
  clamp_sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
  clamp_sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
  clamp_sampler.MipLODBias = 0.0f;
  clamp_sampler.MaxAnisotropy = 1;
  clamp_sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
  clamp_sampler.BorderColor =
      D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
  clamp_sampler.MinLOD = 0;
  clamp_sampler.MaxLOD = D3D12_FLOAT32_MAX;
  clamp_sampler.ShaderVisibility =
      D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
  clamp_sampler.ShaderRegister = register_id;

  return clamp_sampler;
}

}  // namespace defined_static_sampler
}  // namespace shader
}  // namespace directx
}  // namespace legend