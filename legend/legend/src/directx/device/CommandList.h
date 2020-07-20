#ifndef LEGEND_DIRECTX_DEVICE_COMMANDLIST_H_
#define LEGEND_DIRECTX_DEVICE_COMMANDLIST_H_

/**
 * @file CommandList.h
 * @brief コマンドリストクラス定義
 */

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
   * @param device デバイス
   * @param command_queue コマンドキュー
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(ID3D12Device* device, ID3D12CommandQueue* command_queue);
  /**
   * @brief コマンドリストを閉じる
   * @return 閉じることに成功したらtrueを返す
   */
  bool Close();

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

#endif  //! LEGEND_DIRECTX_DEVICE_COMMANDLIST_H_
