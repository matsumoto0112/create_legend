#include "src/util/resource/model.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource {

//コンストラクタ
Model::Model() {}

//デストラクタ
Model::~Model() {}

//読み込み
bool Model::Load(const std::wstring& name) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto command_list = directx::device::CommandList();
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  auto model_path = Path::GetInstance()->model();
  auto model = std::make_shared<draw::Model>();
  if (!model->Init(model_path / name, command_list)) {
    MY_LOG(L"モデルの読み込みに失敗しました。");
    return false;
  }

  if (!command_list.Close()) {
    return false;
  }

  device.ExecuteCommandList({command_list});
  device.WaitExecute();
  return Register(name, model);
}

}  // namespace resource
}  // namespace util
}  // namespace legend