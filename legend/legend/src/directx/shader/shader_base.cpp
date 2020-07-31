#include "src/directx/shader/shader_base.h"

#include "src/util/byte_reader.h"

namespace legend {
namespace directx {
namespace shader {

bool ShaderBase::Init(IDirectXAccessor& accessor,
                      const std::filesystem::path& filepath) {
  this->shader_code_ = util::byte_reader::Read(filepath);
  if (this->shader_code_.empty()) {
    return false;
  }

  return true;
}
}  // namespace shader
}  // namespace directx
}  // namespace legend