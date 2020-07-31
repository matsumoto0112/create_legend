#include "src/util/resource/vertex_shader.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource {

//コンストラクタ
VertexShader::VertexShader() {}

//デストラクタ
VertexShader::~VertexShader() { resources_.clear(); }

//読み込み
bool VertexShader::Load(id::VertexShader key,
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

}  // namespace resource
}  // namespace util
}  // namespace legend