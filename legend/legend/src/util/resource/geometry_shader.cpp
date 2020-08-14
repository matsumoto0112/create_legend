#include "src/util/resource/geometry_shader.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource {

//�R���X�g���N�^
GeometryShader::GeometryShader() {}

//�f�X�g���N�^
GeometryShader::~GeometryShader() {}

//�ǂݍ���
bool GeometryShader::Load(const std::wstring& name) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto shader_path = Path::GetInstance()->shader();

  auto geometry_shader = std::make_shared<directx::shader::GeometryShader>();
  if (!geometry_shader->Init(device, shader_path / name)) {
    MY_LOG(L"�W�I���g���V�F�[�_�[�̏������Ɏ��s���܂����B");
    return false;
  }

  return Register(name, geometry_shader);
}
}  // namespace resource
}  // namespace util
}  // namespace legend