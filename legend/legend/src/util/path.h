#ifndef LEGEND_UTIL_PATH_H_
#define LEGEND_UTIL_PATH_H_

/**
 * @file path.h
 * @brief パス管理クラス定義
 */

#include "src/util/singleton.h"

namespace legend {
namespace util {
/**
 * @brief パス管理
 */
class Path : public Singleton<Path> {
 public:
  /**
   * @brief 実行ファイルのあるパスを取得する
   */
  std::filesystem::path exe() const { return exe_filepath_; }
  /**
   * @brief シェーダーファイルのあるフォルダのパスを取得する
   */
  std::filesystem::path shader() const { return shader_filepath_; }
  /**
   * @brief テクスチャのあるフォルダのパスを取得する
   */
  std::filesystem::path texture() const { return texture_filepath_; }

 protected:
  Path();
  ~Path();

 private:
  //! 実行ファイルのパス
  std::filesystem::path exe_filepath_;
  //! シェーダーファイルのパス
  std::filesystem::path shader_filepath_;
  //! テクスチャのファイルパス
  std::filesystem::path texture_filepath_;
};

}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_PATH_H_
