#include "src/util/byte_reader.h"

#include <fstream>

namespace legend {
namespace util {
namespace byte_reader {
std::vector<u8> Read(const std::filesystem::path& filepath) {
  std::ifstream ifs(filepath.c_str(), std::ios::binary | std::ios::in);
  if (!ifs) {
    return std::vector<u8>();
  }

  ifs.seekg(0, std::ios::end);
  std::vector<u8> res(ifs.tellg());
  ifs.seekg(0, std::ios::beg);
  ifs.read((char*)(res.data()), res.size());

  return res;
}
}  // namespace byte_reader
}  // namespace util
}  // namespace legend