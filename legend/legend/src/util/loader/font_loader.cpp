#include "src/util/loader/font_loader.h"

namespace legend {
namespace util {
namespace loader {

const std::unordered_map<std::wstring, std::wstring> FontLoader::PAIRS = {
    {L"みかちゃん", L"みかちゃん"}};

std::wstring FontLoader::Load(const std::filesystem::path& filename) {
  const std::wstring font_filename = filename.filename().replace_extension();
  MY_ASSERTION(PAIRS.find(font_filename) != PAIRS.end(),
               L"%sが登録されていません。", font_filename.c_str());

  if (std::find(loaded_font_filenames_.begin(), loaded_font_filenames_.end(),
                filename) != loaded_font_filenames_.end()) {
    MY_LOG(L"登録済みフォント情報が再登録されました。");
    return font_filename;
  }

  int ret = AddFontResourceExW(filename.c_str(), FR_PRIVATE, nullptr);
  if (ret == 0) {
    MY_LOG(L"フォント情報が登録できませんでした。");
    return L"";
  }

  loaded_font_filenames_.emplace_back(filename);
  return font_filename;
}

FontLoader::FontLoader() {}

FontLoader::~FontLoader() {
  for (auto&& font : loaded_font_filenames_) {
    RemoveFontResourceExW(font.c_str(), FR_PRIVATE, nullptr);
  }
}

}  // namespace loader
}  // namespace util
}  // namespace legend