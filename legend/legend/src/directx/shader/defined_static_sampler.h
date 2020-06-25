#ifndef LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_
#define LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_

/**
 * @file defined_static_sampler.h
 * @brief よく使うStaticSamplerを定義するファイル
 */

namespace legend {
namespace directx {
namespace shader {
namespace defined_sampler {

/**
 * @brief StaticSamplerを作成する
 * @param register_number シェーダーのレジスター番号
 * @param filter フィルター
 * @param address_mode テクスチャの範囲外参照時の処理方法
 * @return 上記の設定によって作られたサンプラー
 */
D3D12_STATIC_SAMPLER_DESC CreateStaticSampler(
    u32 register_number, D3D12_FILTER filter,
    D3D12_TEXTURE_ADDRESS_MODE address_mode);

}  // namespace defined_sampler
}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_
