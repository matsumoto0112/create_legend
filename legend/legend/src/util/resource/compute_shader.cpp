#include "src/util/resource/compute_shader.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource {

//コンストラクタ
ComputeShader::ComputeShader() {}

//デストラクタ
ComputeShader::~ComputeShader() {}

//読み込み
bool ComputeShader::Load(const std::wstring& name) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto shader_path = Path::GetInstance()->shader();

  auto compute_shader = std::make_shared<directx::shader::ComputeShader>();
  if (!compute_shader->Init(device, shader_path / name)) {
    MY_LOG(L"コンピュートシェーダーの初期化に失敗しました。");
    return false;
  }

  return Register(name, compute_shader);
}
}  // namespace resource
}  // namespace util
}  // namespace legend