#ifndef LEGEND_UTIL_LOADER_FONT_LOADER_H_
#define LEGEND_UTIL_LOADER_FONT_LOADER_H_

/**
 * @file font_loader.h
 * @brief ÉtÉHÉìÉgì«Ç›çûÇ›
 */

#include "src/util/singleton.h"

namespace legend {
namespace util {
namespace loader {

class FontLoader : public Singleton<FontLoader> {
 public:
  std::wstring Load(const std::filesystem::path& filename);

 protected:
  FontLoader();
  ~FontLoader();

 private:
  std::vector<std::wstring> loaded_font_filenames_;

  static const std::unordered_map<std::wstring, std::wstring> PAIRS;
};
}  // namespace loader
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_LOADER_FONT_LOADER_H_
