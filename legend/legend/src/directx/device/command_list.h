#ifndef LEGEND_DIRECTX_DEVICE_COMMAND_LIST_H_
#define LEGEND_DIRECTX_DEVICE_COMMAND_LIST_H_

/**
 * @file CommandList.h
 * @brief コマンドリストクラス定義
 */

#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace device {
/**
 * @brief コマンドリストクラス
 */
class CommandList {
 public:
  /**
   * @brief コンストラクタ
   */
  CommandList();
  /**
   * @brief デストラクタ
   */
  ~CommandList();
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param type コマンドリストの種類
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, D3D12_COMMAND_LIST_TYPE type);
  /**
   * @brief コマンドリストを閉じる
   * @return 閉じることに成功したらtrueを返す
   */
  bool Close();
  /**
   * @brief コマンドリストをリセットする
   * @return リセットに成功したらtrueを返す
   */
  bool Reset();

 public:
  /**
   * @brief コマンドリストを取得する
   */
  inline ID3D12GraphicsCommandList4* GetCommandList() const {
    return command_list_.Get();
  }
  /**
   * @brief コマンドアロケータを取得する
   */
  inline ID3D12CommandAllocator* GetCommandAllocator() const {
    return command_allocator_.Get();
  }

 private:
  //! アロケータ
  ComPtr<ID3D12CommandAllocator> command_allocator_;
  //! コマンドリスト
  ComPtr<ID3D12GraphicsCommandList4> command_list_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_COMMAND_LIST_H_
