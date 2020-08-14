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
bool VertexShader::Load(const std::wstring& name) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto shader_path = Path::GetInstance()->shader();

  auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
  if (!vertex_shader->Init(device, shader_path / name)) {
    MY_LOG(L"頂点シェーダーの初期化に失敗しました。");
    return false;
  }

  return Register(name, vertex_shader);
}

}  // namespace resource
}  // namespace util
}  // namespace legend