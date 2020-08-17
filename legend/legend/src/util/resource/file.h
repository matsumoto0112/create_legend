#ifndef LEGEND_UTIL_RESOURCE_FILE_H
#define LEGEND_UTIL_RESOURCE_FILE_H

/**
 * @file file.h
 * @brief
 */
namespace legend {
namespace util {
namespace resource {
class File {
 public:
  File();
  ~File();

 private:
  std::vector<char*> data_;
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_FILE_H
