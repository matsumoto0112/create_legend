#include "src/util/resource_manager/vertex_shader_resource_manager.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource_manager {

//�R���X�g���N�^
VertexShaderResourceManager::VertexShaderResourceManager() {}

//�f�X�g���N�^
VertexShaderResourceManager::~VertexShaderResourceManager() {
  resources_.clear();
}

//������
bool VertexShaderResourceManager::Init() {
  resources_.clear();
  return true;
}

//�ǂݍ���
bool VertexShaderResourceManager::Load(VertexShaderID key,
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
bool VertexShaderResourceManager::Unload(VertexShaderID key) {
  MY_ASSERTION(IsLoaded(key), L"���o�^�̃L�[���폜����悤�Ƃ��Ă��܂��B");
  return resources_.erase(key) == 1;
}

}  // namespace resource_manager
}  // namespace util
}  // namespace legend