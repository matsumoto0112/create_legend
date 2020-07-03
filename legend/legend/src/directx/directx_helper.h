#ifndef LEGEND_DIRECTX_DIRECTX_HELPER_H_
#define LEGEND_DIRECTX_DIRECTX_HELPER_H_

/**
 * @file directx_helper.h
 * @brief DirectX�n�w���p�[�֐��Q
 */

namespace legend {
namespace directx {
namespace directx_helper {
/**
 * @brief �e�N�X�`���̃t�H�[�}�b�g����s�N�Z���̃������T�C�Y���擾����
 * @param format ���ׂ�t�H�[�}�b�g
 * @return �t�H�[�}�b�g�ɑΉ������������T�C�Y
 */
constexpr u32 CalcPixelSizeFromFormat(DXGI_FORMAT format) {
  switch (format) {
    case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
      // RGBA���ꂼ��1Byte�ō��v4Byte
      return 4;
    case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
      return 4;
    default:
      MY_LOG(L"����`�̃t�H�[�}�b�g���I������܂����B");
      return UINT_MAX;
  }
}

}  // namespace directx_helper
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX_HELPER_H_
