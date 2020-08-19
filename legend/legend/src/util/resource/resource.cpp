#include "src/util/resource/resource.h"

#include "src/util/path.h"

namespace legend {
namespace util {
namespace resource {

//�R���X�g���N�^
Resource::Resource() {}

//�f�X�g���N�^
Resource::~Resource() {}

//������
bool Resource::Init() {
  archive_loader_.Init(util::Path::GetInstance()->exe() / "assets" /
                       "archive.bin");
  if (!pixel_shader_.Init(&archive_loader_)) return false;
  if (!vertex_shader_.Init(&archive_loader_)) return false;
  if (!geometry_shader_.Init(&archive_loader_)) return false;
  if (!compute_shader_.Init(&archive_loader_)) return false;
  if (!pipeline_.Init(&archive_loader_)) return false;
  if (!model_.Init(&archive_loader_)) return false;
  if (!texture_.Init(&archive_loader_)) return false;

  return true;
}

}  // namespace resource
}  // namespace util
}  // namespace legend