#include "src/directx/shader/defined_static_sampler.h"

namespace legend {
namespace directx {
namespace shader {
namespace defined_sampler {
/**
 * @brief スタティックサンプラーを作成する
 * @param register_number シェーダーのレジスター番号
 * @param filter フィルターの種類
 * @param address_mode テクスチャの範囲外参照時の対応法
 * @return 設定をもとに作られたサンプラー
 */
D3D12_STATIC_SAMPLER_DESC CreateStaticSampler(
    u32 register_number, D3D12_FILTER filter,
    D3D12_TEXTURE_ADDRESS_MODE address_mode) {
  return D3D12_STATIC_SAMPLER_DESC{
      filter,
      address_mode,
      address_mode,
      address_mode,
      0.0f,
      0,
      D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_NEVER,
      D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
      0.0f,
      D3D12_FLOAT32_MAX,
      register_number,
      0,
      D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL};
}
}  // namespace defined_sampler
}  // namespace shader
}  // namespace directx
}  // namespace legend