#ifndef LEGEND_DIRECTX_DESCRIPTOR_HANDLE_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HANDLE_H_

/**
 * @file DescriptorHandle.h
 * @brief ディスクリプタハンドル構造体定義
 */

namespace legend {
namespace directx {
/**
 * @brief ディスクリプタハンドル構造体
 * @details 二つのハンドル CPU・GPUをまとめた構造体
 */
struct DescriptorHandle {
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
};

}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HANDLE_H_
