#ifndef LEGEND_DIRECTX_BUFFER_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_BUFFER_H_

/**
 * @file buffer.h
 * @brief バッファ管理クラス定義
 */

#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @brief CreateCommittedResourceで作成されるリソース管理クラス
 */
class CommittedResource {
 public:
  struct TextureBufferDesc {
    std::wstring name;
    DXGI_FORMAT format;
    u32 width;
    u32 height;
    D3D12_RESOURCE_FLAGS flags;
    D3D12_CLEAR_VALUE clear_value;

    TextureBufferDesc(const std::wstring& name = L"",
                      DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN,
                      u32 width = 0, u32 height = 0,
                      D3D12_RESOURCE_FLAGS flags =
                          D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE,
                      D3D12_CLEAR_VALUE clear_value = CD3DX12_CLEAR_VALUE())
        : name(name),
          format(format),
          width(width),
          height(height),
          flags(flags),
          clear_value(clear_value){};
  };

 public:
  /**
   * @brief コンストラクタ
   */
  CommittedResource();
  /**
   * @brief デストラクタ
   */
  ~CommittedResource();
  /**
   * @brief バッファとして初期化する
   * @param accessor DirectX12デバイスアクセサ
   * @param buffer_size バッファのメモリサイズ
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool InitAsBuffer(IDirectXAccessor& accessor, u64 buffer_size,
                    const std::wstring& name);
  bool InitAsTex2D(IDirectXAccessor& accessor, const TextureBufferDesc& desc);
  bool InitFromBuffer(IDirectXAccessor& accessor, ComPtr<ID3D12Resource> buffer,
                      D3D12_RESOURCE_STATES first_state);
  /**
   * @brief 状態を遷移させる
   * @param accessor DirectX12デバイスアクセサ
   * @param next_state 次の状態
   */
  void Transition(IDirectXAccessor& accessor, D3D12_RESOURCE_STATES next_state);

  /**
   * @brief リソースを取得する
   */
  ID3D12Resource* GetResource() const { return resource_.Get(); }
  /**
   * @brief リソースにデータを書き込む
   * @param data 書き込むデータ
   * @return 書き込みに成功したらtrueを返す
   */
  bool WriteResource(const void* data);
  /**
   * @brief バッファサイズを取得する
   */
  u64 GetBufferSize() const { return buffer_size_; }

 public:
  /**
   * @brief テクスチャなどに利用する2つの
   * @param accessor DirectX12デバイスアクセサ
   * @param dest_resource　書き込み先のリソース
   * @param immediate_resource 一時的なリソース
   * @param data 書き込むデータ
   * @param row 1行のメモリサイズ (width * sizeof(Pixel))
   * @param slice 全体のメモリサイズ (row * height)
   */
  static void UpdateSubresource(IDirectXAccessor& accessor,
                                CommittedResource* dest_resource,
                                CommittedResource* immediate_resource,
                                const void* data, u64 row, u64 slice);

 private:
  //! 管理しているリソース
  ComPtr<ID3D12Resource> resource_;
  //! バッファのメモリサイズ
  u64 buffer_size_;
  //! リソースの状態
  D3D12_RESOURCE_STATES current_state_;
};  // namespace buffer

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_BUFFER_H_
