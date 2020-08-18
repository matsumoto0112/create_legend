#ifndef LEGEND_UTIL_PIPELINE_INITIALIZER_H
#define LEGEND_UTIL_PIPELINE_INITIALIZER_H

/**
 * @file pipeline_initializer.h
 * @brief 今回のアプリケーションで使用するパイプラインの初期化機能定義
 */

namespace legend {
namespace util {

/**
 * @brief パイプラインの初期化機能クラス
 */
class PipelineInitializer {
 public:
  /**
   * @brief 初期化
   * @param filepath パイプラインデータ定義ファイルのassetsからの相対パス
   */
  static bool Init(const std::filesystem::path& filepath);
};

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_PIPELINE_INITIALIZER_H
