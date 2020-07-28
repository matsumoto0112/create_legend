#include "src/util/resource/pipeline.h"

namespace legend {
namespace util {
namespace resource {

//�R���X�g���N�^
Pipeline::Pipeline() {}

//�f�X�g���N�^
Pipeline::~Pipeline() {}

//������
bool Pipeline::Init() {
  resources_.clear();
  return true;
}

//�j��
bool Pipeline::Unload(PipelineID key) {
  MY_ASSERTION(IsLoaded(key), L"���o�^�̃L�[���폜����悤�Ƃ��Ă��܂��B");
  return resources_.erase(key) == 1;
}

}  // namespace resource
}  // namespace util
}  // namespace legend