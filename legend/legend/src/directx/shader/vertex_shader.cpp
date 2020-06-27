#include "src/directx/shader/vertex_shader.h"

#include "src/util/byte_reader.h"

namespace legend {
namespace directx {
namespace shader {

//�R���X�g���N�^
VertexShader::VertexShader() {}

//�f�X�g���N�^
VertexShader::~VertexShader() {}

//������
bool VertexShader::Init(DirectX12Device& device,
                        const std::filesystem::path& filepath,
                        const std::vector<D3D12_INPUT_ELEMENT_DESC>& elements) {
  this->shader_code_ = util::byte_reader::Read(filepath);
  if (this->shader_code_.empty()) {
    return false;
  }

  this->elements_ = elements;
  return true;
}
}  // namespace shader
}  // namespace directx
}  // namespace legend
