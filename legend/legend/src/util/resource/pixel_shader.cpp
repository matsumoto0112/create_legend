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
bool PixelShader::Load(PixelShaderID key,
                       const std::filesystem::path& filepath) {
  MY_ASSERTION(!IsLoaded(key), L"�o�^�ς݂̃L�[���ēo�^����悤�Ƃ��Ă��܂��B");
  auto vs = std::make_shared<directx::shader::PixelShader>();
  if (!vs->Init(game::GameDevice::GetInstance()->GetDevice(), filepath)) {
    MY_LOG(L"�s�N�Z���V�F�[�_�[�̏������Ɏ��s���܂����B");
    return false;
  }

  resources_.emplace(key, vs);
  return true;
}

}  // namespace resource
}  // namespace util
}  // namespace legend