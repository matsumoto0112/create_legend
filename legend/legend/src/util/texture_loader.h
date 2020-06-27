#ifndef LEGEND_UTIL_TEXTURE_LOADER_H_
#define LEGEND_UTIL_TEXTURE_LOADER_H_

/**
 * @file texture_loader.h
 * @brief テクスチャ読み込みクラス定義
 */

namespace legend {
namespace util {

namespace texture_loader {
/**
 * @brief 読み込んだテクスチャデータ
 */
struct LoadedTextureData {
  //! テクスチャ名
  std::wstring name;
  //! テクスチャ幅
  u32 width;
  //! テクスチャ高さ
  u32 height;
  //! ピクセル配列
  std::vector<u8> pixels;

  /**
   * @brief デフォルトコンストラクタ
   */
  LoadedTextureData() : name(L""), width(0), height(0), pixels() {}
};
/**
 * @brief テクスチャを読み込む
 * @param filename 読み込むファイル名
 * @return 読み込んだ　テクスチャデータ
 */
LoadedTextureData Load(const std::filesystem::path& filename);

}  // namespace texture_loader

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_TEXTURE_LOADER_H_
