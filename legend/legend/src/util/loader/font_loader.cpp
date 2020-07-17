#include "src/util/loader/font_loader.h"

namespace legend {
namespace util {
namespace loader {

const std::unordered_map<std::wstring, std::wstring> FontLoader::PAIRS = {
    {L"みかちゃん", L"みかちゃん"}};

//読み込む
std::wstring FontLoader::Load(const std::filesystem::path& filename) {
  //拡張子なしのファイル名を取り出す
  const std::wstring font_filename = filename.filename().replace_extension();
  MY_ASSERTION(PAIRS.find(font_filename) != PAIRS.end(),
               L"%sが登録されていません。", font_filename.c_str());

  //すでに読み込み済みなら何もしない
  if (std::find(loaded_font_filenames_.begin(), loaded_font_filenames_.end(),
                filename) != loaded_font_filenames_.end()) {
    MY_LOG(L"登録済みフォント情報が再登録されました。");
    return font_filename;
  }

  //読み込めたフォント数が返されるので0なら読み込めなかったとき
  int ret = AddFontResourceExW(filename.c_str(), FR_PRIVATE, nullptr);
  if (ret == 0) {
    MY_LOG(L"フォント情報が登録できませんでした。");
    return L"";
  }

  loaded_font_filenames_.emplace_back(filename);
  return font_filename;
}

//コンストラクタ
FontLoader::FontLoader() {}

//デストラクタ
FontLoader::~FontLoader() {
  //読み込んだフォントを開放する
  for (auto&& font : loaded_font_filenames_) {
    RemoveFontResourceExW(font.c_str(), FR_PRIVATE, nullptr);
  }
}

}  // namespace loader
}  // namespace util
}  // namespace legend