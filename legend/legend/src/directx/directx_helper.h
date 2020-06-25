#ifndef LEGEND_DIRECTX_DIRECTX_HELPER_H_
#define LEGEND_DIRECTX_DIRECTX_HELPER_H_

/**
 * @file directx_helper.h
 * @brief DirectX�n�w���p�[�֐��Q
 */

namespace legend {
namespace directx {
namespace util {
constexpr u32 CalcPixelSizeFromFormat(DXGI_FORMAT format) {
  switch (format) {
    case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
      return 4;
    default:
      MY_LOG(L"����`�̃t�H�[�}�b�g���I������܂����B");
      return UINT_MAX;
  }
}

}  // namespace util
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX_HELPER_H_
