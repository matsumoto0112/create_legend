#include "src/util/resource/geometry_shader.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource {

//コンストラクタ
GeometryShader::GeometryShader() {}

//デストラクタ
GeometryShader::~GeometryShader() {}

//読み込む
bool GeometryShader::Load(const std::wstring& name) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto shader_path = Path::GetInstance()->shader();

  auto geometry_shader = std::make_shared<directx::shader::GeometryShader>();
  if (!geometry_shader->Init(device, shader_path / name)) {
    MY_LOG(L"ジオメトリシェーダーの初期化に失敗しました。");
    return false;
  }

  return Register(name, geometry_shader);
}
}  // namespace resource
}  // namespace util
}  // namespace legend