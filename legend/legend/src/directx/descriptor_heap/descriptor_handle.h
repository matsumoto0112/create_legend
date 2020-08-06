#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_DESCRIPTOR_HANDLE_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_DESCRIPTOR_HANDLE_H_

/**
 * @file descriptor_handle.h
 * @brief ディスクリプタハンドル構造体定義
 */

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief ディスクリプタハンドル構造体
 * @details 二つのハンドル CPU・GPUをまとめた構造体
 */
struct DescriptorHandle final {
  //! CPUハンドル
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  //! GPUハンドル
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
};

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_DESCRIPTOR_HANDLE_H_
