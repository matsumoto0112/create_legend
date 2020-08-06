#include "src/util/resource/model.h"

namespace legend {
namespace util {
namespace resource {

//�R���X�g���N�^
Model::Model() {}

//�f�X�g���N�^
Model::~Model() {}

//�ǂݍ���
bool Model::Load(ModelID key, const std::filesystem::path& filepath,
                 directx::device::CommandList& command_list) {
  MY_ASSERTION(!IsLoaded(key), L"�o�^�ς݂̃L�[���ēo�^����悤�Ƃ��Ă��܂��B");

  auto model = std::make_shared<draw::Model>();
  if (!model->Init(filepath, command_list)) {
    MY_LOG(L"���f���̓ǂݍ��݂Ɏ��s���܂����B");
    return false;
  }

  resources_.emplace(key, model);
  return true;
}

}  // namespace resource
}  // namespace util
}  // namespace legend