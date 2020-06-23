#include "src/directx/shader/vertex_shader.h"

#include "src/directx/shader/shader_defines.h"

namespace legend {
namespace directx {
namespace shader {

//コンストラクタ
VertexShader::VertexShader() {}

//デストラクタ
VertexShader::~VertexShader() {}

//初期化
bool VertexShader::Init(DirectX12Device& device,
                        const std::filesystem::path& filepath,
                        const std::vector<D3D12_INPUT_ELEMENT_DESC>& elements) {
  if (FAILED(D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "VSMain",
                                "vs_5_0", COMPILE_FLAGS, 0, &vertex_shader_,
                                nullptr))) {
    MY_LOG(L"CompileFromFile VertexShader failed");
    return false;
  }

  this->elements_ = elements;
  return true;
}
}  // namespace shader
}  // namespace directx
}  // namespace legend
