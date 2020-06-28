#include "src/util/loader/texture_loader.h"

namespace legend {
namespace util {
namespace loader {
namespace texture_loader {

//�e�N�X�`���̓ǂݍ���
LoadedTextureData Load(const std::filesystem::path& filename) {
  //�e�N�X�`����ǂݍ���
  int x, y, comp;
  stbi_uc* begin =
      stbi_load(filename.generic_string().c_str(), &x, &y, &comp, 4);

  LoadedTextureData res = {};
  res.width = x;
  res.height = y;
  // std::vector(start,end)�̓R�s�[�ō\�z�����̂ł�����쐬�������begin���J�����Ă����v
  res.pixels = std::vector<u8>(begin, begin + x * y * 4);
  res.name = filename.filename().replace_extension();

  stbi_image_free(begin);

  return res;
}

}  // namespace texture_loader
}  // namespace loader
}  // namespace util
}  // namespace legend