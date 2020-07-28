#include "src/util/resource/vertex_shader.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource {

//�R���X�g���N�^
VertexShader::VertexShader() {}

//�f�X�g���N�^
VertexShader::~VertexShader() {
  resources_.clear();
}

//������
bool VertexShader::Init() {
  resources_.clear();
  return true;
}

//�ǂݍ���
bool VertexShader::Load(VertexShaderID key,
                                       const std::filesystem::path& filepath) {
  MY_ASSERTION(!IsLoaded(key), L"�o�^�ς݂̃L�[���ēo�^����悤�Ƃ��Ă��܂��B");

  auto vs = std::make_shared<directx::shader::VertexShader>();
  if (!vs->Init(game::GameDevice::GetInstance()->GetDevice(), filepath)) {
    MY_LOG(L"���_�V�F�[�_�[�̏������Ɏ��s���܂����B");
    return false;
  }

  resources_.emplace(key, vs);
  return true;
}

//�폜
bool VertexShader::Unload(VertexShaderID key) {
  MY_ASSERTION(IsLoaded(key), L"���o�^�̃L�[���폜����悤�Ƃ��Ă��܂��B");
  return resources_.erase(key) == 1;
}

}  // namespace resource
}  // namespace util
}  // namespace legend