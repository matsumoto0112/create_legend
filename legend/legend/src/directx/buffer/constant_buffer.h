#ifndef LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_

/**
 * @file constant_buffer.h
 * @brief コンスタントバッファクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
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
   * @brief コピーコンストラクタ
   */
  ConstantBuffer(const ConstantBuffer& other);
  /**
   * @brief コピー演算子
   */
  ConstantBuffer& operator=(const ConstantBuffer& other);
  /**
   * @brief ムーブコンストラクタ
   */
  ConstantBuffer(ConstantBuffer&& other) noexcept;
  /**
   * @brief ムーブ代入演算子
   */
  ConstantBuffer& operator=(ConstantBuffer&& other) noexcept;
  /**
   * @brief 状態を全てリセットする
   */
  void Reset();
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param handle ディスクリプタハンドル
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor,
            descriptor_heap::DescriptorHandle handle, const std::wstring& name);

  /**
   * @brief 現在のコンスタントバッファ構造体の参照を取得する
   */
  T& GetStagingRef() { return staging_; }
  /**
   * @brief 現在のコンスタントバッファ構造体のデータを取得する
   */
  T GetStaging() const { return staging_; }
  /**
   * @brief 現在の状態をGPUに更新する
   */
  void UpdateStaging() const;
  /**
   * @brief ハンドルを登録する
   * @param accessor DirectX12デバイスアクセサ
   * @param register_num シェーダーのレジスター番号
   */
  void RegisterHandle(device::IDirectXAccessor& accessor,
                      u32 register_num) const;

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
  descriptor_heap::DescriptorHandle resource_handle_;
  //! コンスタントバッファ構造体
  T staging_;
  //! CPU上のリソースの書き込み領域
  void* resource_begin_;
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
      buffer_aligned_size_{0} {}

//デストラクタ
template <class T>
inline ConstantBuffer<T>::~ConstantBuffer() {
  Reset();
}

//コピーコンストラクタ
template <class T>
inline ConstantBuffer<T>::ConstantBuffer(const ConstantBuffer& other) {
  *this = other;
}

//代入演算子
template <class T>
inline ConstantBuffer<T>& ConstantBuffer<T>::operator=(
    const ConstantBuffer& other) {
  if (this != &other) {
    this->resource_ = other.resource_;
    this->resource_handle_ = other.resource_handle_;
    this->staging_ = other.staging_;
    this->resource_begin_ = nullptr;
    this->buffer_aligned_size_ = other.buffer_aligned_size_;

    if (this->resource_.GetResource()) {
      this->WriteStart();
    }
  }
  return *this;
}

//ムーブコンストラクタ
template <class T>
inline ConstantBuffer<T>::ConstantBuffer(ConstantBuffer&& other) noexcept {
  *this = std::move(other);
}

//ムーブ演算子
template <class T>
inline ConstantBuffer<T>& ConstantBuffer<T>::operator=(
    ConstantBuffer&& other) noexcept {
  if (this != &other) {
    this->resource_ = other.resource_;
    this->resource_handle_ = other.resource_handle_;
    this->staging_ = other.staging_;
    this->resource_begin_ = nullptr;
    this->buffer_aligned_size_ = other.buffer_aligned_size_;

    if (this->resource_.GetResource()) {
      this->WriteStart();
    }
  }
  return *this;
}

//リセット
template <class T>
inline void ConstantBuffer<T>::Reset() {
  WriteEnd();

  resource_.Reset();
  resource_handle_ = descriptor_heap::DescriptorHandle{};
  staging_ = T{};
  buffer_aligned_size_ = 0;
}

//初期化
template <class T>
inline bool ConstantBuffer<T>::Init(device::IDirectXAccessor& accessor,
                                    descriptor_heap::DescriptorHandle handle,
                                    const std::wstring& name) {
  Reset();

  //バッファは一定の大きさにアラインメントされている必要があるのでアラインメントの計算をする
  buffer_aligned_size_ = math::util::AlignPow2(
      sizeof(T), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);

  // InitAsBufferはUploadヒープで作成するのでステートはGENERIC_READである必要がある
  const CommittedResource::BufferDesc desc{
      name, buffer_aligned_size_,
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ};
  if (!resource_.InitAsBuffer(accessor, desc)) {
    return false;
  }

  this->resource_handle_ = handle;

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
inline void ConstantBuffer<T>::RegisterHandle(
    device::IDirectXAccessor& accessor, u32 register_num) const {
  accessor.RegisterHandle(register_num, shader::ResourceType::CBV,
                          this->resource_handle_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_H_
