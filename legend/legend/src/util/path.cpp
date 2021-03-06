#include "src/util/path.h"

namespace {
constexpr legend::i32 MAX_SIZE = 1024;
}  // namespace

namespace legend {
namespace util {

Path::Path() {
  wchar_t path[MAX_SIZE];
  DWORD ret = GetModuleFileName(nullptr, path, sizeof(path));
  this->exe_filepath_ = path;
  this->exe_filepath_.remove_filename();

  this->shader_filepath_ = exe_filepath_ / "assets" / "shaders";
  this->texture_filepath_ = exe_filepath_ / "assets" / "textures";
  this->model_filepath_ = exe_filepath_ / "assets" / "models";
  this->config_filepath_ = exe_filepath_ / "config";
}

Path::~Path() {}

}  // namespace util
}  // namespace legend