#include "src/util/loader/texture_loader.h"

namespace legend {
namespace util {
namespace loader {
namespace texture_loader {

//テクスチャの読み込み
LoadedTextureData Load(const std::filesystem::path& filename) {
  //テクスチャを読み込む
  int x, y, comp;
  stbi_uc* begin =
      stbi_load(filename.generic_string().c_str(), &x, &y, &comp, 4);

  LoadedTextureData res = {};
  res.width = x;
  res.height = y;
  // std::vector(start,end)はコピーで構築されるのでこれを作成した後はbeginを開放しても大丈夫
  res.pixels = std::vector<u8>(begin, begin + x * y * 4);
  res.name = filename.filename().replace_extension();

  stbi_image_free(begin);

  return res;
}

}  // namespace texture_loader
}  // namespace loader
}  // namespace util
}  // namespace legend