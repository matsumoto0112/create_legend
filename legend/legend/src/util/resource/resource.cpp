#include "src/util/resource/resource.h"

namespace legend {
namespace util {
namespace resource {

//�R���X�g���N�^
Resource::Resource() {}

//�f�X�g���N�^
Resource::~Resource() {}

//������
bool Resource::Init() {
  if (!pixel_shader_.Init()) return false;
  if (!vertex_shader_.Init()) return false;
  if (!pipeline_.Init()) return false;

  return true;
}

}  // namespace resource
}  // namespace util
}  // namespace legend