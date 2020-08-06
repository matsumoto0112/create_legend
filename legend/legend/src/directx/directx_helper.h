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
    case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
      return sizeof(float);
    default:
      MY_LOG(L"未定義のフォーマットが選択されました。");
      return UINT_MAX;
  }
}

/**
 * @brief HRESULTを文字列に変換する
 */
inline std::wstring HrToWString(HRESULT hr) {
  char buf[64] = {};
  sprintf_s(buf, 64, "HRESULT of 0x%08X", static_cast<u32>(hr));
  return util::string_util::String_2_WString(buf);
}

/**
 * @brief HRESULT型が成功しているか判定し、失敗していたらログを出力するする
 * @return 成功していたらtrueを返す
 */
inline bool Succeeded(HRESULT hr) {
  if (SUCCEEDED(hr)) {
    return true;
  }
  MY_LOG(L"HRESULT of %s", HrToWString(hr).c_str());
  return false;
}

/**
 * @brief HRESULT型が成功しているか判定し、失敗していたらログを出力するする
 * @return 失敗していたらtrueを返す
 */
inline bool Failed(HRESULT hr) {
  if (FAILED(hr)) {
    MY_LOG(L"HRESULT of %s", HrToWString(hr).c_str());
    return true;
  }
  return false;
}

/**
 * @brief DXRをサポートしているか
 * @param adapter 現在有効なアダプター
 * @return サポートしていればtrueを返す
 */
inline bool IsDirectXRaytracingSupported(IDXGIAdapter1* adapter) {
  ComPtr<ID3D12Device> test_device;
  D3D12_FEATURE_DATA_D3D12_OPTIONS5 feature_support_data = {};

  //デバイスが作れるなら作る
  //ここで作れなければそもそもここに来る前に終了しているだろうが念のため判定する
  const bool succeeded_create_device = SUCCEEDED(
      D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
                        IID_PPV_ARGS(&test_device)));
  if (!succeeded_create_device) return false;

  // GPUのサポート状態を取得する
  const bool succeed_check_feature_support =
      SUCCEEDED(test_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5,
                                                 &feature_support_data,
                                                 sizeof(feature_support_data)));
  if (!succeed_check_feature_support) return false;

  //レイトレーシングをサポートしているかどうか判定
  return feature_support_data.RaytracingTier !=
         D3D12_RAYTRACING_TIER::D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
}

}  // namespace directx_helper
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX_HELPER_H_
