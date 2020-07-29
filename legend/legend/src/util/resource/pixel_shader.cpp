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
bool PixelShader::Load(PixelShaderID key,
                       const std::filesystem::path& filepath) {
  MY_ASSERTION(!IsLoaded(key), L"登録済みのキーが再登録されようとしています。");
  auto vs = std::make_shared<directx::shader::PixelShader>();
  if (!vs->Init(game::GameDevice::GetInstance()->GetDevice(), filepath)) {
    MY_LOG(L"ピクセルシェーダーの初期化に失敗しました。");
    return false;
  }

  resources_.emplace(key, vs);
  return true;
}

}  // namespace resource
}  // namespace util
}  // namespace legend