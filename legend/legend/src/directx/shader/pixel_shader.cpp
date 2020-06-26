#include "src/directx/shader/pixel_shader.h"

#include "src/util/byte_reader.h"

namespace legend {
namespace directx {
namespace shader {

//�R���X�g���N�^
PixelShader::PixelShader() {}

//�f�X�g���N�^
PixelShader::~PixelShader() {}

//������
bool PixelShader::Init(DirectX12Device& device,
                       const std::filesystem::path& filepath) {
  this->pixel_shader_data = util::byte_reader::Read(filepath);
  if (this->pixel_shader_data.empty()) {
    return false;
  }

  return true;
}
}  // namespace shader
}  // namespace directx
}  // namespace legend
