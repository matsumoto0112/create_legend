#ifndef LEGEND_UTIL_TEXTURE_LOADER_H_
#define LEGEND_UTIL_TEXTURE_LOADER_H_

/**
 * @file texture_loader.h
 * @brief テクスチャ読み込みクラス定義
 */

namespace legend {
namespace util {

namespace texture_loader {
struct LoadedTextureData {
  std::wstring name;
  u32 width;
  u32 height;
  std::vector<u8> pixels;
};

LoadedTextureData Load(const std::filesystem::path& filename);

}  // namespace texture_loader

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_TEXTURE_LOADER_H_
