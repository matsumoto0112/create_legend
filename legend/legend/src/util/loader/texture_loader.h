#ifndef LEGEND_UTIL_LOADER_TEXTURE_LOADER_H_
#define LEGEND_UTIL_LOADER_TEXTURE_LOADER_H_

/**
 * @file texture_loader.h
 * @brief �e�N�X�`���ǂݍ��݃N���X��`
 */

namespace legend {
namespace util {
namespace loader {

namespace texture_loader {
/**
 * @brief �ǂݍ��񂾃e�N�X�`���f�[�^
 */
struct LoadedTextureData {
  //! �e�N�X�`����
  std::wstring name;
  //! �e�N�X�`����
  u32 width;
  //! �e�N�X�`������
  u32 height;
  //! �s�N�Z���z��
  std::vector<u8> pixels;

  /**
   * @brief �f�t�H���g�R���X�g���N�^
   */
  LoadedTextureData() : name(L""), width(0), height(0), pixels() {}
};
/**
 * @brief �e�N�X�`����ǂݍ���
 * @param filename �ǂݍ��ރt�@�C����
 * @return �ǂݍ��񂾃e�N�X�`���f�[�^
 */
LoadedTextureData Load(const std::filesystem::path& filename);
/**
 * @brief ����������e�N�X�`����ǂݍ���
 * @param pixel_data �e�N�X�`�����̃s�N�Z���f�[�^
 * @return �ǂݍ��񂾃e�N�X�`���f�[�^
 */
LoadedTextureData LoadFromMemory(const std::vector<u8>& pixel_data);

}  // namespace texture_loader

}  // namespace loader
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_LOADER_TEXTURE_LOADER_H_