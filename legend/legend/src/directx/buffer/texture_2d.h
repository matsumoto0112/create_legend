#ifndef LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_
#define LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_

/**
 * @file texture_2d.h
 * @brief 2Dテクスチャクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class Texture2D
 * @brief 2Dテクスチャクラス
 */
class Texture2D {
 public:
  /**
   * @struct Desc
   * @brief テクスチャデスク
   */
  struct Desc {
    //! フォーマット
    DXGI_FORMAT format;
    //! 幅
    u32 width;
    //! 高さ
    u32 height;
    //! ハンドル
    descriptor_heap::DescriptorHandle handle;
    //! リソース名
    std::wstring name;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  Texture2D();
  /**
   * @brief デストラクタ
   */
  ~Texture2D();
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param desc テクスチャデスク
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor, const Desc& desc);
  /**
   * @brief テクスチャを読み込みつつ初期化する
   * @param accessor DirectXデバイスアクセサ
   * @param command_list コマンドリスト
   * @param filename ファイル名
   * @param handle テクスチャハンドル
   * @return 初期化に成功したらtrueを返す
   * @details リソースのコピーのためにコマンドリストが必要
   */
  bool InitAndWrite(device::IDirectXAccessor& accessor,
                    device::CommandList& command_list,
                    const std::filesystem::path& filename,
                    const descriptor_heap::DescriptorHandle& handle);
  /**
   * @brief テクスチャを書き込みつつ初期化する
   * @param accessor DirectXデバイスアクセサ
   * @param command_list コマンドリスト
   * @param format テクスチャのフォーマット
   * @param data テクスチャデータ
   * @param handle テクスチャハンドル
   * @param filename リソース名
   * @return 初期化に成功したらtrueを返す
   * @details リソースのコピーのためにコマンドリストが必要
   */
  bool InitAndWrite(device::IDirectXAccessor& accessor,
                    device::CommandList& command_list, DXGI_FORMAT format,
                    const std::vector<u8>& data,
                    const descriptor_heap::DescriptorHandle& handle,
                    const std::wstring& name);
  /**
   * @brief テクスチャデータを書き込む
   * @param command_list コマンドリスト
   * @param data テクスチャデータ
   */
  void WriteResource(device::CommandList& command_list, const void* data);

  /**
   * @brief ヒープに自身を追加する
   * @param accessor DirectXデバイスアクセサ
   * @param register_num シェーダーのレジスター番号
   */
  void RegisterHandle(device::IDirectXAccessor& accessor, u32 register_num);

 public:
  /**
   * @brief テクスチャの幅を取得する
   */
  inline u32 GetWidth() const { return width_; }
  /**
   * @brief テクスチャの高さを取得する
   */
  inline u32 GetHeight() const { return height_; }

 private:
  /**
   * @brief テクスチャバッファを初期化する
   * @param accessor DirectXデバイスアクセサ
   * @param desc テクスチャデスク
   * @return 初期化に成功したらtrueを返す
   */
  bool InitTexBuffer(device::IDirectXAccessor& accessor, const Desc& desc);

 private:
  //! テクスチャデータ
  buffer::CommittedResource texture_;
  //! コピー用のテクスチャリソース
  buffer::CommittedResource texture_immediate_;
  //! テクスチャのフォーマット
  DXGI_FORMAT format_;
  //! テクスチャの幅
  u32 width_;
  //! テクスチャの高さ
  u32 height_;
  //! ハンドル
  descriptor_heap::DescriptorHandle handle_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_
