#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_TYPE_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_TYPE_H_

/**
 * @file descriptor_heap_type.h
 * @brief ディスクリプタヒープ種類定義
 */

namespace legend {
namespace directx {

/**
 * @brief ディスクリプタヒープの種類
 */
enum class DescriptorHeapType {
  CBV_SRV_UAV,
  RTV,
  DSV,
  SAMPLER,
};

/**
 * @brief ディスクリプタヒープの使用フラグ
 */
enum class DescriptorHeapFlag {
  NONE,
  SHADER_VISIBLE,
};
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_TYPE_H_
