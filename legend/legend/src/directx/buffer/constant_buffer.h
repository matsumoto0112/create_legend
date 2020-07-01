#ifndef LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_

/**
 * @file constant_buffer.h
 * @brief コンスタントバッファクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/directx_accessor.h"
#include "src/math/math_util.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class ConstantBuffer
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
   * @brief 状態を全てリセットする
   */
  void Reset();
  /**
   * @brief 初期化
   * @param accessor DirectX12デバイスアクセサ
   * @param register_num シェーダーのレジスター番号
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, u32 register_num,
            const std::wstring& name);

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
  void SetToHeap(IDirectXAccessor& accessor) const;

 private:
  /**
   * @brief 書き込み開始
   */
  bool WriteStart();
  /**
   * @brief 書き込み終了
   */
  void WriteEnd();

 private:
  //! リソース
  CommittedResource resource_;
  //! ハンドル
  DescriptorHandle resource_handle_;
  //! コンスタントバッファ構造体
  T staging_;
  //! CPU上のリソースの書き込み領域
  void* resource_begin_;
  //! シェーダーのレジスター番号
  u32 register_num_;
  //! アライメントされたバッファサイズ
  u32 buffer_aligned_size_;
};

//コンストラクタ
template <class T>
inline ConstantBuffer<T>::ConstantBuffer()
    : resource_{},
      resource_handle_{},
      staging_{},
      resource_begin_{nullptr},
      register_num_{0},
      buffer_aligned_size_{0} {}

//デストラクタ
template <class T>
inline ConstantBuffer<T>::~ConstantBuffer() {
  Reset();
}

template <class T>
inline void ConstantBuffer<T>::Reset() {
  WriteEnd();

  resource_.Reset();
  resource_handle_ = DescriptorHandle{};
  staging_ = T{};
  u32 buffer_aligned_size_ = 0;
  register_num_ = 0;
}

//初期化
template <class T>
inline bool ConstantBuffer<T>::Init(IDirectXAccessor& accessor,
                                    u32 register_num,
                                    const std::wstring& name) {
  Reset();

  buffer_aligned_size_ = math::util::AlignPow2(
      sizeof(T), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
  if (!resource_.InitAsBuffer(accessor, buffer_aligned_size_, name)) {
    return false;
  }

  this->register_num_ = register_num;
  this->resource_handle_ = accessor.GetHandle(DescriptorHeapType::CBV_SRV_UAV);

  D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
  cbv_desc.BufferLocation = resource_.GetResource()->GetGPUVirtualAddress();
  cbv_desc.SizeInBytes = buffer_aligned_size_;
  accessor.GetDevice()->CreateConstantBufferView(
      &cbv_desc, this->resource_handle_.cpu_handle_);

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
inline void ConstantBuffer<T>::SetToHeap(IDirectXAccessor& accessor) const {
  accessor.SetToGlobalHeap(this->register_num_, ResourceType::Cbv,
                           this->resource_handle_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
