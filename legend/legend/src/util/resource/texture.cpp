#include "src/util/resource/texture.h"

#include "src/game/game_device.h"
#include "src/util/loader/texture_loader.h"

namespace legend {
namespace util {
namespace resource {

//コンストラクタ
Texture::Texture() {}

//デストラクタ
Texture::~Texture() {}

//読み込み
bool Texture::Load(const std::wstring& name) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto command_list = directx::device::CommandList();
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  auto texture_path = Path::GetInstance()->texture();
  auto texture = std::make_shared<directx::buffer::Texture2D>();
  if (!texture->InitAndWrite(
          device, command_list, 0, texture_path / name,
          device.GetLocalHandle(directx::descriptor_heap::heap_parameter::
                                    LocalHeapID::GLOBAL_ID))) {
    MY_LOG(L"テクスチャの初期化に失敗しました。");
    return false;
  }

  if (!command_list.Close()) {
    return false;
  }
  device.ExecuteCommandList({command_list});
  device.WaitExecute();

  return Register(name, texture);
}

}  // namespace resource
}  // namespace util
}  // namespace legend