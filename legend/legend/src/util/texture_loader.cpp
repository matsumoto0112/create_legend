#include "src/util/texture_loader.h"

namespace legend {
namespace util {
namespace texture_loader {

LoadedTextureData Load(const std::filesystem::path& filename) {
  int x, y, comp;
  stbi_uc* begin =
      stbi_load(filename.generic_string().c_str(), &x, &y, &comp, 4);

  LoadedTextureData res = {};
  res.width = x;
  res.height = y;
  res.pixels = std::vector<u8>(begin, begin + x * y * 4);
  res.name = filename.filename().replace_extension();

  stbi_image_free(begin);

  return res;
}

}  // namespace texture_loader
}  // namespace util
}  // namespace legend