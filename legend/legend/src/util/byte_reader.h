#ifndef LEGEND_UTIL_BYTE_READER_H_
#define LEGEND_UTIL_BYTE_READER_H_

/**
 * @file byte_reader.h
 * @brief バイトデータ読み込み関数定義
 */

namespace legend {
namespace util {
namespace byte_reader {
/**
 * @brief バイトデータの読み込み
 * @param filepath ファイルへのパス
 * @return 読み込んだデータ
 */
std::vector<u8> Read(const std::filesystem::path& filepath);
}  // namespace byte_reader
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_BYTE_READER_H_
