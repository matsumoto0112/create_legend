#ifndef LEGEND_DIRECTX_DIRECTX_HELPER_H_
#define LEGEND_DIRECTX_DIRECTX_HELPER_H_

/**
 * @file directx_helper.h
 * @brief DirectX系ヘルパー関数群
 */

namespace legend {
namespace directx {
namespace directx_helper {
/**
 * @brief テクスチャのフォーマットからピクセルのメモリサイズを取得する
 * @param format 調べるフォーマット
 * @return フォーマットに対応したメモリサイズ
 */
constexpr u32 CalcPixelSizeFromFormat(DXGI_FORMAT format) {
  switch (format) {
    case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
      // RGBAそれぞれ1Byteで合計4Byte
      return 4;
    case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
      return 4;
    default:
      MY_LOG(L"未定義のフォーマットが選択されました。");
      return UINT_MAX;
  }
}

}  // namespace directx_helper
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX_HELPER_H_
