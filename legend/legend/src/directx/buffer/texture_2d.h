#ifndef LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_
#define LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_

/**
 * @file texture_2d.h
 * @brief 2Dテクスチャクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @brief 2Dテクスチャクラス
 */
class Texture2D {
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
   * @param device DirectX12デバイス
   * @param register_num シェーダーのレジスター番号
   * @param format テクスチャのフォーマット
   * @param width テクスチャの幅
   * @param height テクスチャの高さ
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(DirectX12Device& device, u32 register_num, DXGI_FORMAT format,
            u32 width, u32 height, descriptor_heap::DescriptorHandle handle,
            const std::wstring& name);
  /**
   * @brief テクスチャを読み込みつつ初期化する
   * @param device DirectX12デバイス
   * @param register_num シェーダーのレジスター番号
   * @param filename ファイル名
   * @return 初期化に成功したらtrueを返す
   */
  bool InitAndWrite(DirectX12Device& device, u32 register_num,
                    const std::filesystem::path& filename,
                    descriptor_heap::DescriptorHandle handle);
  /**
   * @brief テクスチャを書き込みつつ初期化する
   * @param device DirectX12デバイス
   * @param register_num シェーダーのレジスター番号
   * @param data テクスチャデータ
   * @param filename ファイル名
   * @return 初期化に成功したらtrueを返す
   */
  bool InitAndWrite(DirectX12Device& device, u32 register_num,
                    const std::vector<u8>& data,
                    descriptor_heap::DescriptorHandle handle,
                    const std::wstring& name);
  /**
   * @brief テクスチャデータを書き込む
   * @param device DirectX12デバイス
   * @param data テクスチャデータ
   */
  void WriteResource(DirectX12Device& device, const void* data);

  /**
   * @brief ヒープに自身を追加する
   * @param device DirectX12デバイス
   */
  void SetToHeap(DirectX12Device& device);
  /**
   * @brief レジスター番号を指定してヒープに自身を追加する
   * @param device DirectX12デバイス
   * @param overwrite_register_num 上書きするレジスター番号
   */
  void SetToHeap(DirectX12Device& device, u32 overwrite_register_num);

 public:
  inline void SetWidth(u32 width) { this->width_ = width; }
  inline u32 GetWidth() const { return width_; }
  inline void SetHeight(u32 height) { this->height_ = height; }
  inline u32 GetHeight() const { return height_; }

 private:
  /**
   * @brief テクスチャバッファを初期化する
   * @param device DirectX12デバイス
   * @param register_num シェーダーのレジスター番号
   * @param format テクスチャのフォーマット
   * @param width テクスチャの幅
   * @param height テクスチャの高さ
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool InitTexBuffer(DirectX12Device& device, u32 register_num,
                     DXGI_FORMAT format, u32 width, u32 height,
                     descriptor_heap::DescriptorHandle handle,
                     const std::wstring& name);

 private:
  //! テクスチャデータ
  buffer::CommittedResource texture_;
  //! コピー用のテクスチャリソース
  buffer::CommittedResource texture_immediate_;
  //! シェーダーのレジスター番号
  u32 register_num_;
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
