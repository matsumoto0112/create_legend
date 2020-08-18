#include "src/util/loader/texture_loader.h"

#include "src/game/game_device.h"
#include "src/libs/stb_image.h"
#include "src/util/path.h"

namespace legend {
namespace util {
namespace loader {
namespace texture_loader {

//テクスチャの読み込み
LoadedTextureData Load(const std::filesystem::path& filename) {
  //テクスチャを読み込む
  int x, y, comp;
  stbi_uc* begin = stbi_load(filename.generic_string().c_str(), &x, &y, &comp,
                             STBI_rgb_alpha);
  if (!begin) {
    MY_LOG(L"%sの読み込みに失敗しました", filename.c_str());
    return LoadedTextureData{};
  }

  LoadedTextureData res = {};
  res.width = x;
  res.height = y;
  // std::vector(start,end)はコピーで構築されるのでこれを作成した後はbeginを開放しても大丈夫
  res.pixels = std::vector<u8>(begin, begin + x * y * 4);
  res.name = filename.filename().replace_extension();

  stbi_image_free(begin);
  return res;
}

//メモリから読み込む
LoadedTextureData LoadFromMemory(const std::vector<u8>& pixel_data) {
  int w, h, comp;
  const u32 size = static_cast<u32>(pixel_data.size());
  u8* begin = stbi_load_from_memory(pixel_data.data(), size, &w, &h, &comp, 4);
  if (!begin) {
    MY_LOG(L"メモリから復元することに失敗しました");
    return LoadedTextureData{};
  }

  LoadedTextureData res = {};
  res.width = w;
  res.height = h;
  res.pixels = std::vector<u8>(begin, begin + w * h * 4);
  res.name = L"";

  stbi_image_free(begin);

  return res;
}

}  // namespace texture_loader
}  // namespace loader
}  // namespace util
}  // namespace legend