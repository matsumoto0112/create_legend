#include "src/util/loader/glb_loader.h"

#include <GLTFSDK/Deserialize.h>

namespace legend {
namespace util {
namespace loader {

StreamReader::StreamReader() {}

std::shared_ptr<std::istream> StreamReader::GetInputStream(
    const std::string& filepath) const {
  auto stream =
      std::make_shared<std::ifstream>(filepath, std::ios_base::binary);

  if (!stream) return nullptr;
  return (stream);
}

GLBLoader::GLBLoader() : stream_reader_(std::make_unique<StreamReader>()) {}

GLBLoader::~GLBLoader() {}

LoadedGLBModelData GLBLoader::Load(const std::filesystem::path& filename) {
  using namespace Microsoft::glTF;

  std::shared_ptr<std::istream> glb_stream =
      stream_reader_->GetInputStream(filename.generic_string());
  std::unique_ptr<GLBResourceReader> glb_resource_reader =
      std::make_unique<GLBResourceReader>(std::move(stream_reader_),
                                          std::move(glb_stream));

  std::string manifest = glb_resource_reader->GetJson();
  Document document = Deserialize(manifest);

  LoadedGLBModelData res = {};
  res.name = filename.filename().replace_extension();

  for (auto&& mesh : document.meshes.Elements()) {
    for (auto&& prim : mesh.primitives) {
      std::string accessor_id;
      if (prim.TryGetAttributeAccessorId(ACCESSOR_POSITION, accessor_id)) {
        const Accessor& accessor = document.accessors.Get(accessor_id);
        const std::vector<float> data =
            glb_resource_reader->ReadBinaryData<float>(document, accessor);
        const u32 length = static_cast<u32>(data.size() * sizeof(float));
        MY_LOG(L"Prim: %d", length);
        res.positions = data;
      }
      const Accessor& index_accessor =
          document.accessors.Get(prim.indicesAccessorId);
      if (index_accessor.componentType == ComponentType::COMPONENT_UNSIGNED_SHORT) {
        const std::vector<u16> index_data =
            glb_resource_reader->ReadBinaryData<u16>(document, index_accessor);
        res.indices = index_data;
      }
    }
    // MY_LOG(L"Mesh: %s", string_util::String_2_WString(mesh.id));
  }

  return res;
}

}  // namespace loader
}  // namespace util
}  // namespace legend
