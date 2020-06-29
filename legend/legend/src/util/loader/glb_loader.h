#ifndef LEGEND_UTIL_LOADER_GLB_LOADER_H_
#define LEGEND_UTIL_LOADER_GLB_LOADER_H_

#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/GLTF.h>

/**
 * @file glb_loader.h
 * @brief .glbファイルを読み込む
 */

namespace legend {
namespace util {
namespace loader {

/**
 * @brief 読み込んだモデルデータ
 */
struct LoadedMeshData {
  //! モデル名
  std::wstring name;
  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> uvs;
  std::vector<u16> indices;
};

/**
 * @brief 読み込み
 */
class StreamReader final : public Microsoft::glTF::IStreamReader {
 public:
  StreamReader();
  /**
   * @brief filenameをバイナリ形式で読み込む
   * @param filename 読み込むファイル名
   * @return ストリームを構築して返す
   */
  std::shared_ptr<std::istream> GetInputStream(
      const std::string& filename) const override;
};

/**
 * @brief GLBモデルを読み込む
 */
class GLBLoader {
 public:
  /**
   * @brief コンストラクタ
   */
  GLBLoader();
  /**
   * @brief デストラクタ
   */
  ~GLBLoader();
  /**
   * @brief 読み込む
   * @param filename 読み込むファイル名
   * @return 読み込んだデータからモデルの情報を返す
   * @details 現状シングルメッシュしかうまく取得できません
   */
  LoadedMeshData Load(const std::filesystem::path& filename);

 private:
  //! ストリーム読み込み器
  std::unique_ptr<StreamReader> stream_reader_;
};

}  // namespace loader
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_LOADER_GLB_LOADER_H_
