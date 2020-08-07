#ifndef LEGEND_DIRECTX_BUFFER_COMMITTED_RESOURCE_H_
#define LEGEND_DIRECTX_BUFFER_COMMITTED_RESOURCE_H_

/**
 * @file buffer.h
 * @brief バッファ管理クラス定義
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class CommittedResource
 * @brief CreateCommittedResourceで作成されるリソース管理クラス
 */
class CommittedResource {
 public:
  /**
   * @brief バッファデスク
   */
  struct BufferDesc {
    //! リソース名
    std::wstring name;
    //! バッファの大きさ
    u64 buffer_size;
    //! リソースの初期状態
    D3D12_RESOURCE_STATES init_states;
  };

  /**
   * @brief テクスチャバッファデスク
   */
  struct Tex2DDesc {
    //! リソース名
    std::wstring name;
    //! テクスチャフォーマット
    DXGI_FORMAT format;
    //! テクスチャ幅
    u32 width;
    //! テクスチャ高さ
    u32 height;
    //! リソースの使用フラグ
    D3D12_RESOURCE_FLAGS flags;
    //! 初期化が必要な時の初期化値
    D3D12_CLEAR_VALUE clear_value;
    //! リソースの初期状態
    D3D12_RESOURCE_STATES init_states;
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
   * @brief 状態を全てリセットする
   */
  void Reset();
  /**
   * @brief バッファとして初期化する
   * @param accessor DirectXデバイスアクセサ
   * @param desc バッファデスク
   */
  bool InitAsBuffer(device::IDirectXAccessor& accessor, const BufferDesc& desc);
  /**
   * @brief 2Dテクスチャとして初期化する
   * @param accessor DirectXデバイスアクセサ
   * @param desc テクスチャデスク
   */
  bool InitAsTex2D(device::IDirectXAccessor& accessor, const Tex2DDesc& desc);
  /**
   * @brief バッファをコピーして初期化する
   * @param accessor DirectXデバイスアクセサ
   * @param buffer コピー元のバッファ
   * @param init_states リソースの初期状態
   * @param name リソース名
   */
  bool InitFromBuffer(device::IDirectXAccessor& accessor,
                      ComPtr<ID3D12Resource> buffer,
                      D3D12_RESOURCE_STATES init_states,
                      const std::wstring& name);
  /**
   * @brief 状態を遷移させる
   * @param command_list コマンドリスト
   * @param next_state 次の状態
   */
  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_state);

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
   * @brief テクスチャなどに利用する2つのリソースを利用したコピーをする
   * @param command_list コマンドリスト
   * @param dest_resource　書き込み先のリソース
   * @param immediate_resource 一時的なリソース
   * @param data 書き込むデータ
   * @param row 1行のメモリサイズ (width * sizeof(Pixel))
   * @param slice 全体のメモリサイズ (row * height)
   */
  static void UpdateSubresource(device::CommandList& command_list,
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
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_COMMITTED_RESOURCE_H_
