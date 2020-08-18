#include "src/util/loader/archive_loader.h"

#include "src/util/stl_extend.h"
#include "src/util/string_util.h"

namespace legend {
namespace util {
namespace loader {

//コンストラクタ
ArchiveLoader::ArchiveLoader() {}

//デストラクタ
ArchiveLoader::~ArchiveLoader() { Finalize(); }

//初期化
bool ArchiveLoader::Init(const std::filesystem::path& filepath) {
  //ファイルから4バイト読み込みi32として解釈し、取得する
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

  //末尾から4バイト目に表の開始位置がある
  ifs_.seekg(-4, std::ifstream::end);
  int table_begin = GetInt(&ifs_);

  //リソースファイルのファイル数を取得する
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

//読み込み
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

//終了処理
void ArchiveLoader::Finalize() {
  if (ifs_) ifs_.close();
}

}  // namespace loader
}  // namespace util
}  // namespace legend