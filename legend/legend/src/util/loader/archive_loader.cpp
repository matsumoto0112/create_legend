#include "src/util/loader/archive_loader.h"

#include "src/util/stl_extend.h"
#include "src/util/string_util.h"

namespace legend {
namespace util {
namespace loader {

//�R���X�g���N�^
ArchiveLoader::ArchiveLoader() {}

//�f�X�g���N�^
ArchiveLoader::~ArchiveLoader() { Finalize(); }

//������
bool ArchiveLoader::Init(const std::filesystem::path& filepath) {
  //�t�@�C������4�o�C�g�ǂݍ���i32�Ƃ��ĉ��߂��A�擾����
  auto GetInt = [](std::ifstream* wfs) {
    unsigned char buf[4];
    wfs->read(reinterpret_cast<char*>(buf), 4);
    int a = buf[0];
    a |= (buf[1] << 8);
    a |= (buf[2] << 16);
    a |= (buf[3] << 24);
    return a;
  };

  ifs_ = std::ifstream(filepath, std::ifstream::binary | std::ifstream::in);
  if (!ifs_) {
    return false;
  }

  //��������4�o�C�g�ڂɕ\�̊J�n�ʒu������
  ifs_.seekg(-4, std::ifstream::end);
  int table_begin = GetInt(&ifs_);

  //���\�[�X�t�@�C���̃t�@�C�������擾����
  ifs_.seekg(table_begin, std::ifstream::beg);
  int file_num = GetInt(&ifs_);

  for (int i = 0; i < file_num; i++) {
    int pos = GetInt(&ifs_);
    int size = GetInt(&ifs_);
    int name_length = GetInt(&ifs_);
    std::string name(name_length, '\0');
    ifs_.read(name.data(), name_length);
    load_files_.emplace(string_util::String_2_WString(name), Entity{pos, size});
  }

  return true;
}

//�ǂݍ���
std::vector<u8> ArchiveLoader::Load(
    const std::filesystem::path& filename_path) {
  const std::wstring filename = filename_path.generic_wstring();
  if (!Exist(load_files_, filename)) {
    return std::vector<u8>();
  }

  Entity e = load_files_.at(filename);
  ifs_.seekg(e.position, std::ifstream::beg);
  std::vector<u8> res(e.size);
  ifs_.read(reinterpret_cast<char*>(&res[0]), e.size);
  return res;
}

//�I������
void ArchiveLoader::Finalize() {
  if (ifs_) ifs_.close();
}

}  // namespace loader
}  // namespace util
}  // namespace legend