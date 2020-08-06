#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_DESCRIPTOR_HANDLE_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_DESCRIPTOR_HANDLE_H_

/**
 * @file descriptor_handle.h
 * @brief �f�B�X�N���v�^�n���h���\���̒�`
 */

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief �f�B�X�N���v�^�n���h���\����
 * @details ��̃n���h�� CPU�EGPU���܂Ƃ߂��\����
 */
struct DescriptorHandle final {
  //! CPU�n���h��
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  //! GPU�n���h��
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
};

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_DESCRIPTOR_HANDLE_H_
