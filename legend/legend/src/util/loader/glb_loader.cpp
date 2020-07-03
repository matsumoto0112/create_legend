#include "src/util/loader/glb_loader.h"

#include <GLTFSDK/Deserialize.h>

#include <tuple>

namespace legend {
namespace util {
namespace loader {
using namespace Microsoft::glTF;

StreamReader::StreamReader() {}

std::shared_ptr<std::istream> StreamReader::GetInputStream(
    const std::string& filepath) const {
  auto stream =
      std::make_shared<std::ifstream>(filepath, std::ios_base::binary);

  if (!stream) return nullptr;
  return stream;
}

//�R���X�g���N�^
GLBLoader::GLBLoader()
    : stream_reader_(nullptr), glb_resource_reader_(nullptr), document_() {}

//�f�X�g���N�^
GLBLoader::~GLBLoader() {}

bool GLBLoader::Load(const std::filesystem::path& filename) {
  stream_reader_ = std::make_unique<StreamReader>();
  try {
    //.glb�p�̓ǂݍ��݋@���쐬����
    std::shared_ptr<std::istream> glb_stream =
        stream_reader_->GetInputStream(filename.generic_string());
    glb_resource_reader_ = std::make_unique<GLBResourceReader>(
        std::move(stream_reader_), std::move(glb_stream));
    //.json�`���Ŏ擾�ł���̂Ńf�V���A���C�Y���ăf�[�^���擾����
    std::string manifest = glb_resource_reader_->GetJson();
    document_ = Deserialize(manifest);
  } catch ([[maybe_unused]] const std::exception& e) {
    MY_LOG(string_util::String_2_WString(e.what()));
    return false;
  }

  return true;
}

//���_���̎擾
u32 GLBLoader::GetVertexNum(u32 mesh_index, u32 primitive_index) const {
  //���W�������_���ƈ�v���Ă���Ɖ��肷��
  return static_cast<u32>(
      GetPosition(mesh_index, primitive_index).size() /
      GetPositionComponentSize(mesh_index, primitive_index));
}

//���_���W
std::vector<float> GLBLoader::GetPosition(u32 mesh_index,
                                          u32 primitive_index) const {
  const MeshPrimitive& primitive = GetMesh(mesh_index, primitive_index);
  return GetAttribute(primitive, ACCESSOR_POSITION);
}

//���_���W�̗v�f��
u32 GLBLoader::GetPositionComponentSize(u32 mesh_index,
                                        u32 primitive_index) const {
  const MeshPrimitive& primitive = GetMesh(mesh_index, primitive_index);
  return GetAttributeComponentSize(primitive, ACCESSOR_POSITION);
}

//�@��
std::vector<float> GLBLoader::GetNormal(u32 mesh_index,
                                        u32 primitive_index) const {
  const MeshPrimitive& primitive = GetMesh(mesh_index, primitive_index);
  return GetAttribute(primitive, ACCESSOR_NORMAL);
}

//�@���̗v�f��
u32 GLBLoader::GetNormalComponentSize(u32 mesh_index,
                                      u32 primitive_index) const {
  const MeshPrimitive& primitive = GetMesh(mesh_index, primitive_index);
  return GetAttributeComponentSize(primitive, ACCESSOR_NORMAL);
}

// UV
std::vector<float> GLBLoader::GetUV(u32 mesh_index, u32 primitive_index) const {
  const MeshPrimitive& primitive = GetMesh(mesh_index, primitive_index);
  return GetAttribute(primitive, ACCESSOR_TEXCOORD_0);
}

// UV�̗v�f��
u32 GLBLoader::GetUVComponentSize(u32 mesh_index, u32 primitive_index) const {
  const MeshPrimitive& primitive = GetMesh(mesh_index, primitive_index);
  return GetAttributeComponentSize(primitive, ACCESSOR_TEXCOORD_0);
}

//�ڐ�
std::vector<float> GLBLoader::GetTangent(u32 mesh_index,
                                         u32 primitive_index) const {
  const MeshPrimitive& primitive = GetMesh(mesh_index, primitive_index);
  return GetAttribute(primitive, ACCESSOR_TANGENT);
}

//�ڐ��̗v�f��
u32 GLBLoader::GetTangentComponentSize(u32 mesh_index,
                                       u32 primitive_index) const {
  const MeshPrimitive& primitive = GetMesh(mesh_index, primitive_index);
  return GetAttributeComponentSize(primitive, ACCESSOR_TANGENT);
}

//�C���f�b�N�X�z��
std::vector<u16> GLBLoader::GetIndex(u32 mesh_index,
                                     u32 primitive_index) const {
  const MeshPrimitive& mesh = GetMesh(mesh_index, primitive_index);
  //�C���f�b�N�X�����擾����
  const Accessor& index_accessor =
      document_.accessors.Get(mesh.indicesAccessorId);

  // u16�Ŋi�[����Ă��邩�m�F
  if (index_accessor.componentType == ComponentType::COMPONENT_UNSIGNED_SHORT) {
    return glb_resource_reader_->ReadBinaryData<u16>(document_, index_accessor);
  }
  return std::vector<u16>();
}

std::vector<u8> GLBLoader::GetAlbedo() const {
  const Image& image = document_.images[0];
  return glb_resource_reader_->ReadBinaryData(document_, image);
}

std::vector<u8> GLBLoader::GetNormalMap() const { return std::vector<u8>(); }

//���b�V�����擾����
const Microsoft::glTF::MeshPrimitive& GLBLoader::GetMesh(
    u32 mesh_index, u32 primitive_index) const {
  const Mesh& mesh = document_.meshes[mesh_index];
  return mesh.primitives[primitive_index];
}

//�A�N�Z�T���擾����
bool GLBLoader::GetAccessor(const Microsoft::glTF::MeshPrimitive& mesh,
                            const char* accessor_name,
                            Microsoft::glTF::Accessor* accessor) const {
  std::string accessor_id;
  if (mesh.TryGetAttributeAccessorId(accessor_name, accessor_id)) {
    *accessor = document_.accessors.Get(accessor_id);
    return true;
  }
  return false;
}

//�A�g���r���[�g���擾����
std::vector<float> GLBLoader::GetAttribute(
    const Microsoft::glTF::MeshPrimitive& mesh,
    const char* accessor_name) const {
  std::vector<float> res;
  if (Accessor accessor; GetAccessor(mesh, accessor_name, &accessor)) {
    res = glb_resource_reader_->ReadBinaryData<float>(document_, accessor);
  }
  return res;
}

//�A�g���r���[�g�̗v�f�����擾����
u32 GLBLoader::GetAttributeComponentSize(
    const Microsoft::glTF::MeshPrimitive& mesh,
    const char* accessor_name) const {
  if (Accessor accessor; GetAccessor(mesh, accessor_name, &accessor)) {
    switch (accessor.type) {
      case AccessorType::TYPE_VEC2:
        return 2;
      case AccessorType::TYPE_VEC3:
        return 3;
      case AccessorType::TYPE_VEC4:
        return 4;
    }
  }
  return 0;
}

}  // namespace loader
}  // namespace util
}  // namespace legend
