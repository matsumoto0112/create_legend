#ifndef LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_

/**
 * @file constant_buffer.h
 * @brief コンスタントバッファクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx12_device.h"
#include "src/math/math_util.h"

namespace legend {
namespace directx {
namespace buffer {

template <class T>
class ConstantBuffer {
 public:
  ConstantBuffer();
  ~ConstantBuffer();
  bool Init(DirectX12Device& device, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle,
            D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle, const std::wstring& name);
  T& GetStagingRef() { return staging_; }
  void UpdateStaging() const;

  D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() const { return cpu_handle_; }
  D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return gpu_handle_; }

 public:
  //コピー・ムーブ禁止
  ConstantBuffer(const ConstantBuffer&) = delete;
  ConstantBuffer& operator=(const ConstantBuffer&) = delete;
  ConstantBuffer(ConstantBuffer&&) = delete;
  ConstantBuffer& operator=(ConstantBuffer&&) = delete;

 private:
  T staging_;
  u32 buffer_aligned_size_;
  CommittedResource resource_;
  D3D12_CONSTANT_BUFFER_VIEW_DESC view_;
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
  void* resource_begin_;
};

template <class T>
inline ConstantBuffer<T>::ConstantBuffer() {}

template <class T>
inline ConstantBuffer<T>::~ConstantBuffer() {
  if (resource_begin_) {
    resource_.GetResource()->Unmap(0, nullptr);
    resource_begin_ = nullptr;
  }
}

template <class T>
inline bool ConstantBuffer<T>::Init(DirectX12Device& device,
                                    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle,
                                    D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle,
                                    const std::wstring& name) {
  if (resource_begin_) {
    resource_.GetResource()->Unmap(0, nullptr);
    resource_begin_ = nullptr;
  }

  buffer_aligned_size_ = math::util::AlignPow2(
      sizeof(T), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
  if (!resource_.InitAsBuffer(device, buffer_aligned_size_, name)) {
    return false;
  }

  this->cpu_handle_ = cpu_handle;
  this->gpu_handle_ = gpu_handle;
  this->view_.BufferLocation = resource_.GetResource()->GetGPUVirtualAddress();
  this->view_.SizeInBytes = buffer_aligned_size_;
  device.GetDevice()->CreateConstantBufferView(&view_, this->cpu_handle_);

  if (FAILED(resource_.GetResource()->Map(0, nullptr, &resource_begin_))) {
    return false;
  }

  return true;
}

template <class T>
inline void ConstantBuffer<T>::UpdateStaging() const {
  memcpy_s(resource_begin_, buffer_aligned_size_, &staging_, sizeof(T));
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
