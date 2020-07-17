#ifndef LEGEND_UTIL_LOADER_FONT_LOADER_H_
#define LEGEND_UTIL_LOADER_FONT_LOADER_H_

/**
 * @file font_loader.h
 * @brief フォント読み込み
 */

#include "src/util/singleton.h"

namespace legend {
namespace util {
namespace loader {

/**
 * @brief フォント読み込み器
 */
class FontLoader : public Singleton<FontLoader> {
 public:
  /**
   * @brief 読み込む
   * @param filename 読み込むファイル名
   * @return 読み込んだフォント名
   */
  std::wstring Load(const std::filesystem::path& filename);

 protected:
  FontLoader();
  ~FontLoader();

 private:
  //! 読み込み済みフォント名
  std::vector<std::wstring> loaded_font_filenames_;

  //フォントのファイル名とフォント名のmap
  static const std::unordered_map<std::wstring, std::wstring> PAIRS;
};
}  // namespace loader
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_LOADER_FONT_LOADER_H_
