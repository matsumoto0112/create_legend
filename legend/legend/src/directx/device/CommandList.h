#ifndef LEGEND_DIRECTX_DEVICE_COMMANDLIST_H_
#define LEGEND_DIRECTX_DEVICE_COMMANDLIST_H_

/**
 * @file CommandList.h
 * @brief コマンドリストクラス定義
 */

namespace legend {
namespace directx {
namespace device {
class CommandList {
 public:
  CommandList();
  ~CommandList();
  bool Init(ID3D12Device* device, ID3D12CommandQueue* command_queue);

 public:
  inline ID3D12GraphicsCommandList4* GetCommandList() const {
    return command_list_.Get();
  }
  inline ID3D12CommandAllocator* GetCommandAllocator() const {
    return command_allocator_.Get();
  }

 private:
  ComPtr<ID3D12CommandAllocator> command_allocator_;
  ComPtr<ID3D12GraphicsCommandList4> command_list_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_COMMANDLIST_H_
