#ifndef LEGEND_UTIL_LOADER_GLB_LOADER_H_
#define LEGEND_UTIL_LOADER_GLB_LOADER_H_

#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/GLTF.h>

/**
 * @file glb_loader.h
 * @brief .glbÉtÉ@ÉCÉãÇì«Ç›çûÇﬁ
 */

namespace legend {
namespace util {
namespace loader {

struct LoadedGLBModelData {
  std::wstring name;
  std::vector<float> positions;
  std::vector<u16> indices;
};

class StreamReader : public Microsoft::glTF::IStreamReader {
 public:
  StreamReader();
  std::shared_ptr<std::istream> GetInputStream(
      const std::string& filename) const override;
};

class GLBLoader {
 public:
  GLBLoader();
  ~GLBLoader();
  LoadedGLBModelData Load(const std::filesystem::path& filename);

 private:
  std::unique_ptr<StreamReader> stream_reader_;
};

}  // namespace loader
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_LOADER_GLB_LOADER_H_
