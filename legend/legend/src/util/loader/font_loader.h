#ifndef LEGEND_UTIL_LOADER_FONT_LOADER_H_
#define LEGEND_UTIL_LOADER_FONT_LOADER_H_

/**
 * @file font_loader.h
 * @brief �t�H���g�ǂݍ���
 */

#include "src/util/singleton.h"

namespace legend {
namespace util {
namespace loader {

/**
 * @brief �t�H���g�ǂݍ��݊�
 */
class FontLoader : public Singleton<FontLoader> {
 public:
  /**
   * @brief �ǂݍ���
   * @param filename �ǂݍ��ރt�@�C����
   * @return �ǂݍ��񂾃t�H���g��
   */
  std::wstring Load(const std::filesystem::path& filename);

 protected:
  FontLoader();
  ~FontLoader();

 private:
  //! �ǂݍ��ݍς݃t�H���g��
  std::vector<std::wstring> loaded_font_filenames_;

  //�t�H���g�̃t�@�C�����ƃt�H���g����map
  static const std::unordered_map<std::wstring, std::wstring> PAIRS;
};
}  // namespace loader
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_LOADER_FONT_LOADER_H_
