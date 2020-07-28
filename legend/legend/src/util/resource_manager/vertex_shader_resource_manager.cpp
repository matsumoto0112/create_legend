#include "src/util/resource_manager/vertex_shader_resource_manager.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource_manager {

//コンストラクタ
VertexShaderResourceManager::VertexShaderResourceManager() {}

//デストラクタ
VertexShaderResourceManager::~VertexShaderResourceManager() {
  resources_.clear();
}

//初期化
bool VertexShaderResourceManager::Init() {
  resources_.clear();
  return true;
}

//読み込み
bool VertexShaderResourceManager::Load(VertexShaderID key,
                                       const std::filesystem::path& filepath) {
  MY_ASSERTION(!IsLoaded(key), L"登録済みのキーが再登録されようとしています。");

  auto vs = std::make_shared<directx::shader::VertexShader>();
  if (!vs->Init(game::GameDevice::GetInstance()->GetDevice(), filepath)) {
    MY_LOG(L"頂点シェーダーの初期化に失敗しました。");
    return false;
  }

  resources_.emplace(key, vs);
  return true;
}

//削除
bool VertexShaderResourceManager::Unload(VertexShaderID key) {
  MY_ASSERTION(IsLoaded(key), L"未登録のキーが削除されようとしています。");
  return resources_.erase(key) == 1;
}

}  // namespace resource_manager
}  // namespace util
}  // namespace legend