#include "src/directx/shader/pixel_shader.h"

#include "src/directx/shader/shader_defines.h"

namespace legend {
namespace directx {
namespace shader {

//コンストラクタ
PixelShader::PixelShader() {}

//デストラクタ
PixelShader::~PixelShader() {}

//初期化
bool PixelShader::Init(DirectX12Device& device,
                       const std::filesystem::path& filepath) {
  if (FAILED(D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "PSMain",
                                "ps_5_0", COMPILE_FLAGS, 0, &pixel_shader_,
                                nullptr))) {
    MY_LOG(L"CompileFromFile PixelShader failed");
    return false;
  }

  return true;
}
}  // namespace shader
}  // namespace directx
}  // namespace legend
