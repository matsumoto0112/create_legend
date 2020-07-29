#include "src/util/resource/model.h"

namespace legend {
namespace util {
namespace resource {

Model::Model() {}

Model::~Model() {}

bool Model::Init() {
  resources_.clear();
  return true;
}

bool Model::Load(ModelID key, const std::filesystem::path& filepath) {
  MY_ASSERTION(!IsLoaded(key), L"�o�^�ς݂̃L�[���ēo�^����悤�Ƃ��Ă��܂��B");

  auto model = std::make_shared<draw::Model>();
  if (!model->Init(filepath)) {
    MY_LOG(L"���f���̓ǂݍ��݂Ɏ��s���܂����B");
    return false;
  }

  resources_.emplace(key, model);
  return true;
}

bool Model::Unload(ModelID key) {
  MY_ASSERTION(IsLoaded(key), L"���o�^�̃L�[���폜����悤�Ƃ��Ă��܂��B");
  return resources_.erase(key) == 1;
}

}  // namespace resource
}  // namespace util
}  // namespace legend