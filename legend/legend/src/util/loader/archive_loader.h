#ifndef LEGEND_UTIL_LOADER_ARCHIVE_LOADER_H
#define LEGEND_UTIL_LOADER_ARCHIVE_LOADER_H

/**
 * @file archive_loader.h
 * @brief
 */
namespace legend {
namespace util {
namespace loader {

struct Entity {
 public:
  i32 position;
  i32 size;

  Entity(i32 position, i32 size) : position(position), size(size) {}
  ~Entity() {}
};

class ArchiveLoader {
 public:
  ArchiveLoader() {}
  ~ArchiveLoader() {}

  bool Init(const std::filesystem::path& filepath);
  std::vector<u8> Load(const std::filesystem::path& filename);
  void Finalize();

 private:
  std::unordered_map<std::wstring, Entity> load_files_;
  std::ifstream ifs_;
};

}  // namespace loader
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_LOADER_ARCHIVE_LOADER_H
