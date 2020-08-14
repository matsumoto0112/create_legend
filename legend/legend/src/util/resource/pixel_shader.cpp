#include "src/util/resource/pixel_shader.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource {

//コンストラクタ
PixelShader::PixelShader() {}

//デストラクタ
PixelShader::~PixelShader() {}

//読み込む
bool PixelShader::Load(const std::wstring& name) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto shader_path = Path::GetInstance()->shader();

  auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(device, shader_path / name)) {
    MY_LOG(L"ピクセルシェーダーの初期化に失敗しました。");
    return false;
  }

  return Register(name, pixel_shader);
}

}  // namespace resource
}  // namespace util
}  // namespace legend