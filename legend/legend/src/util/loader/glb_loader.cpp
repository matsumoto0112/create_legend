#include "src/util/loader/glb_loader.h"

#include <GLTFSDK/Deserialize.h>

#include <tuple>

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

  //.glb�p�̓ǂݍ��݋@���쐬����
  std::shared_ptr<std::istream> glb_stream =
      stream_reader_->GetInputStream(filename.generic_string());
  std::unique_ptr<GLBResourceReader> glb_resource_reader =
      std::make_unique<GLBResourceReader>(std::move(stream_reader_),
                                          std::move(glb_stream));

  //.json�`���Ŏ擾�ł���̂Ńf�V���A���C�Y���ăf�[�^���擾����
  std::string manifest = glb_resource_reader->GetJson();
  Document document = Deserialize(manifest);

  LoadedMeshData res = {};
  res.name = filename.filename().replace_extension();

  //�A�N�Z�T������f�[�^���擾����
  auto GetAttribute = [](const Document& document, const MeshPrimitive& mesh,
                         const GLBResourceReader& reader,
                         const char* accessor_name, std::vector<float>* vec,
                         u32* size) {
    std::string accessor_id;
    if (mesh.TryGetAttributeAccessorId(accessor_name, accessor_id)) {
      const Accessor& accessor = document.accessors.Get(accessor_id);
      *vec = reader.ReadBinaryData<float>(document, accessor);
      *size = 3;
      if (accessor.type == AccessorType::TYPE_VEC2) *size = 2;
    }
  };

  for (auto&& mesh : document.meshes.Elements()) {
    for (auto&& prim : mesh.primitives) {
      //���_�A�@���AUV���𔲂��o��
      GetAttribute(document, prim, *glb_resource_reader, ACCESSOR_POSITION,
                   &res.positions, &res.position_size);
      GetAttribute(document, prim, *glb_resource_reader, ACCESSOR_NORMAL,
                   &res.normals, &res.normal_size);
      GetAttribute(document, prim, *glb_resource_reader, ACCESSOR_TEXCOORD_0,
                   &res.uvs, &res.uv_size);
      GetAttribute(document, prim, *glb_resource_reader, ACCESSOR_TANGENT,
                   &res.tangents, &res.tangent_size);

      res.vertex_num =
          static_cast<u32>(res.positions.size() / res.position_size);

      //�C���f�b�N�X�����擾����
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

  for (auto&& image : document.images.Elements()) {
    const std::vector<u8> data =
        glb_resource_reader->ReadBinaryData(document, image);
    res.image = data;
  }

  return res;
}

}  // namespace loader
}  // namespace util
}  // namespace legend
