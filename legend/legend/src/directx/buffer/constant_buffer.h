#ifndef LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_

/**
 * @file constant_buffer.h
 * @brief コンスタントバッファクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx12_device.h"
#include "src/directx/heap_manager.h"
#include "src/math/math_util.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @brief コンスタントバッファクラス
 * @tparam T コンスタントバッファとして使用する構造体
 */
template <class T>
class ConstantBuffer {
 public:
  /**
   * @brief コンストラクタ
   */
  ConstantBuffer();
  /**
   * @brief デストラクタ
   */
  ~ConstantBuffer();
  /**
   * @brief 初期化
   * @param device DirectX12デバイス
   * @param register_num シェーダーのレジスター番号
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(DirectX12Device& device, u32 register_num,
            const std::wstring& name);
  /**
   * @brief 書き込み開始
   */
  bool WriteStart();
  /**
   * @brief 書き込み終了
   */
  void WriteEnd();
  /**
   * @brief 現在のコンスタントバッファ構造体の参照を取得する
   */
  T& GetStagingRef() { return staging_; }
  /**
   * @brief 現在の状態をGPUに更新する
   */
  void UpdateStaging() const;
  /**
   * @brief ヒープに自身を追加する
   * @param accessor DirectX12デバイスアクセサ
   */
  void SetToHeap(DirectX12Device& device);

 public:
  //コピー・ムーブ禁止
  ConstantBuffer(const ConstantBuffer&) = delete;
  ConstantBuffer& operator=(const ConstantBuffer&) = delete;
  ConstantBuffer(ConstantBuffer&&) = delete;
  ConstantBuffer& operator=(ConstantBuffer&&) = delete;

 private:
  //! コンスタントバッファ構造体
  T staging_;
  //! アライメントされたバッファサイズ
  u32 buffer_aligned_size_;
  //! シェーダーのレジスター番号
  u32 register_num_;
  //! リソース
  CommittedResource resource_;
  //! コンスタントバッファビュー
  D3D12_CONSTANT_BUFFER_VIEW_DESC view_;
  //! CPUハンドル
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  //! GPUハンドル
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
  //! CPU上のリソースの書き込み領域
  void* resource_begin_;
};

//コンストラクタ
template <class T>
inline ConstantBuffer<T>::ConstantBuffer() {}

//デストラクタ
template <class T>
inline ConstantBuffer<T>::~ConstantBuffer() {
  WriteEnd();
}

//初期化
template <class T>
inline bool ConstantBuffer<T>::Init(DirectX12Device& device, u32 register_num,
                                    const std::wstring& name) {
  WriteEnd();

  buffer_aligned_size_ = math::util::AlignPow2(
      sizeof(T), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
  if (!resource_.InitAsBuffer(device, buffer_aligned_size_, name)) {
    return false;
  }

  DescriptorHandle handle = device.GetHeapManager().GetLocalHandle();
  this->cpu_handle_ = handle.cpu_handle_;
  this->gpu_handle_ = handle.gpu_handle_;
  this->register_num_ = register_num;
  this->view_.BufferLocation = resource_.GetResource()->GetGPUVirtualAddress();
  this->view_.SizeInBytes = buffer_aligned_size_;
  device.GetDevice()->CreateConstantBufferView(&view_, this->cpu_handle_);

  return WriteStart();
}

//書き込み開始
template <class T>
inline bool ConstantBuffer<T>::WriteStart() {
  if (resource_begin_) return true;

  if (FAILED(resource_.GetResource()->Map(0, nullptr, &resource_begin_))) {
    return false;
  }

  return true;
}

//書き込み終了
template <class T>
inline void ConstantBuffer<T>::WriteEnd() {
  if (resource_begin_) {
    resource_.GetResource()->Unmap(0, nullptr);
    resource_begin_ = nullptr;
  }
}

//構造体の更新
template <class T>
inline void ConstantBuffer<T>::UpdateStaging() const {
  memcpy_s(resource_begin_, buffer_aligned_size_, &staging_, sizeof(T));
}

//ヒープにセットする
template <class T>
inline void ConstantBuffer<T>::SetToHeap(DirectX12Device& device) {
  device.GetHeapManager().SetHandleToLocalHeap(this->register_num_,
                                          ResourceType::Cbv, this->cpu_handle_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
