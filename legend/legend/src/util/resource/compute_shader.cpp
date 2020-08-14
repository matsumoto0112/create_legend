#include "src/util/resource/compute_shader.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource {

//�R���X�g���N�^
ComputeShader::ComputeShader() {}

//�f�X�g���N�^
ComputeShader::~ComputeShader() {}

//�ǂݍ���
bool ComputeShader::Load(const std::wstring& name) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto shader_path = Path::GetInstance()->shader();

  auto compute_shader = std::make_shared<directx::shader::ComputeShader>();
  if (!compute_shader->Init(device, shader_path / name)) {
    MY_LOG(L"�R���s���[�g�V�F�[�_�[�̏������Ɏ��s���܂����B");
    return false;
  }

  return Register(name, compute_shader);
}
}  // namespace resource
}  // namespace util
}  // namespace legend