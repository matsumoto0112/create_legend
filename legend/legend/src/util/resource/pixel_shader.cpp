#include "src/util/resource/pixel_shader.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource {

//�R���X�g���N�^
PixelShader::PixelShader() {}

//�f�X�g���N�^
PixelShader::~PixelShader() {}

//�ǂݍ���
bool PixelShader::Load(const std::wstring& name) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto shader_path = Path::GetInstance()->shader();

  auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(device, shader_path / name)) {
    MY_LOG(L"�s�N�Z���V�F�[�_�[�̏������Ɏ��s���܂����B");
    return false;
  }

  return Register(name, pixel_shader);
}

}  // namespace resource
}  // namespace util
}  // namespace legend