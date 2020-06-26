#ifndef LEGEND_DIRECTX_DESCRIPTORHANDLE_H_
#define LEGEND_DIRECTX_DESCRIPTORHANDLE_H_

/**
 * @file DescriptorHandle.h
 * @brief �f�B�X�N���v�^�n���h��
 */

namespace legend {
namespace directx {
struct DescriptorHandle {
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
};
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTORHANDLE_H_
