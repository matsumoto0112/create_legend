#ifndef LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_
#define LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_

/**
 * @file defined_static_sampler.h
 * @brief よく使うStaticSamplerを定義するファイル
 */

namespace legend {
namespace directx {
namespace shader {
namespace defined_static_sampler {

D3D12_STATIC_SAMPLER_DESC Warp(u32 register_id);
D3D12_STATIC_SAMPLER_DESC Clamp(u32 register_id);

}  // namespace defined_static_sampler
}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_
