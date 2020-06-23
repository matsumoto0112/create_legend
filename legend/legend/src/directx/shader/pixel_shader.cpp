#include "src/directx/shader/pixel_shader.h"

namespace legend {
namespace directx {
namespace shader {
PixelShader::PixelShader() {}

PixelShader::~PixelShader() {}

bool PixelShader::Init(DirectX12Device& device, const std::wstring& filepath) {
#if defined(_DEBUG)
  unsigned int compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  unsigned int compile_flags = 0;
#endif

  if (FAILED(D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "PSMain",
                                "ps_5_0", compile_flags, 0, &pixel_shader_,
                                nullptr))) {
    MY_LOG(L"CompileFromFile PixelShader failed");
    return false;
  }

  return true;
}
}  // namespace shader
}  // namespace directx
}  // namespace legend
