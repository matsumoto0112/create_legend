#include "src/util/resource/vertex_shader.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource {

//�R���X�g���N�^
VertexShader::VertexShader() {}

//�f�X�g���N�^
VertexShader::~VertexShader() { resources_.clear(); }

//�ǂݍ���
bool VertexShader::Load(const std::wstring& name) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto shader_path = Path::GetInstance()->shader();

  auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
  if (!vertex_shader->Init(device, shader_path / name)) {
    MY_LOG(L"���_�V�F�[�_�[�̏������Ɏ��s���܂����B");
    return false;
  }

  return Register(name, vertex_shader);
}

}  // namespace resource
}  // namespace util
}  // namespace legend