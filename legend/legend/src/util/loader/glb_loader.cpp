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
  return stream;
}

GLBLoader::GLBLoader() : stream_reader_(std::make_unique<StreamReader>()) {}

GLBLoader::~GLBLoader() {}

LoadedMeshData GLBLoader::Load(const std::filesystem::path& filename) {
  using namespace Microsoft::glTF;

  //.glb用の読み込み機を作成する
  std::shared_ptr<std::istream> glb_stream =
      stream_reader_->GetInputStream(filename.generic_string());
  std::unique_ptr<GLBResourceReader> glb_resource_reader =
      std::make_unique<GLBResourceReader>(std::move(stream_reader_),
                                          std::move(glb_stream));

  //.json形式で取得できるのでデシリアライズしてデータを取得する
  std::string manifest = glb_resource_reader->GetJson();
  Document document = Deserialize(manifest);

  LoadedMeshData res = {};
  res.name = filename.filename().replace_extension();

  //アクセサ名からデータを取得する
  auto GetAttribute = [](const Document& document, const MeshPrimitive& mesh,
                         const GLBResourceReader& reader,
                         const char* accessor_name) {
    std::string accessor_id;
    if (mesh.TryGetAttributeAccessorId(accessor_name, accessor_id)) {
      const Accessor& accessor = document.accessors.Get(accessor_id);
      const std::vector<float> data =
          reader.ReadBinaryData<float>(document, accessor);
      return data;
    }
    return std::vector<float>();
  };

  for (auto&& mesh : document.meshes.Elements()) {
    for (auto&& prim : mesh.primitives) {
      res.positions =
          GetAttribute(document, prim, *glb_resource_reader, ACCESSOR_POSITION);
      res.normals =
          GetAttribute(document, prim, *glb_resource_reader, ACCESSOR_NORMAL);
      res.uvs = GetAttribute(document, prim, *glb_resource_reader,
                             ACCESSOR_TEXCOORD_0);

      const Accessor& index_accessor =
          document.accessors.Get(prim.indicesAccessorId);
      if (index_accessor.componentType ==
          ComponentType::COMPONENT_UNSIGNED_SHORT) {
        const std::vector<u16> index_data =
            glb_resource_reader->ReadBinaryData<u16>(document, index_accessor);
        res.indices = index_data;
      }
    }
  }

  return res;
}

}  // namespace loader
}  // namespace util
}  // namespace legend
