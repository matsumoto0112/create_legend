#include "src/directx/shader/vertex_shader.h"

namespace dx12_lib {
namespace directx {
namespace shader {
VertexShader::VertexShader() {}
VertexShader::~VertexShader() {}
bool VertexShader::Init(DirectX12Device& device, const std::wstring& filepath) {
#if defined(_DEBUG)
  unsigned int compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  unsigned int compile_flags = 0;
#endif

  if (FAILED(D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "VSMain",
                                "vs_5_0", compile_flags, 0, &vertex_shader_,
                                nullptr))) {
    util::debug::Log(L"CompileFromFile VertexShader failed");
    return false;
  }

  return true;
}
}  // namespace shader
}  // namespace directx
}  // namespace dx12_lib
