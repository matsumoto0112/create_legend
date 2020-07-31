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
bool Texture::Load(
    id::Texture key, const std::filesystem::path& filepath, u32 register_num,
    directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  MY_ASSERTION(!IsLoaded(key), L"登録済みのキーが再登録されようとしています。");

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  auto tex = std::make_shared<directx::buffer::Texture2D>();
  if (!tex->InitAndWrite(device, register_num, filepath,
                         device.GetLocalHeapHandle(heap_id))) {
    return false;
  }

  resources_.emplace(key, tex);
  return true;
}

}  // namespace resource
}  // namespace util
}  // namespace legend