#ifndef LEGEND_DIRECTX_FRAME_RESOURCE_H_
#define LEGEND_DIRECTX_FRAME_RESOURCE_H_

/**
 * @file frame_resource.h
 * @brief DirectXデバイスで管理するフレーム単位でのコマンドリストリソース定義
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {

/**
 * @class FrameResource
 * @brief フレームリソース
 */
class FrameResource {
 public:
  /**
   * @brief コンストラクタ
   */
  FrameResource();
  /**
   * @brief デストラクタ
   */
  virtual ~FrameResource();
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor,
            D3D12_COMMAND_LIST_TYPE command_list_type);
  /**
   * @brief リソースの準備をする
   * @return 成功したらtrueを返す
   */
  bool Ready();
  /**
   * @brief リソースを破棄する
   */
  void Destroy();
  /**
   * @brief コマンドリストを取得する
   */
  device::CommandList& GetCommandList() { return command_lists_; }

 public:
  //! フェンス値
  u64 fence_value_;

 private:
  //! コマンドリスト
  device::CommandList command_lists_;
};

}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_FRAME_RESOURCE_H_
