#ifndef LEGEND_DIRECTX_DEVICE_DXGI_ADAPTER_H_
#define LEGEND_DIRECTX_DEVICE_DXGI_ADAPTER_H_

/**
 * @file dxgi_adapter.h
 * @brief DXGIアダプター管理クラス定義
 */

#include "src/directx/device/device_option.h"

namespace legend {
namespace directx {
namespace device {
/**
 * @brief DXGIアダプタークラス
 */
class DXGIAdapter {
 public:
  /**
   * @brief コンストラクタ
   */
  DXGIAdapter();
  /**
   * @brief デストラクタ
   */
  ~DXGIAdapter();
  /**
   * @brief 初期化
   * @param required_option 要求するオプション
   * @param adapter_id_override 上書きするアダプターID
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(DeviceOptionFlags required_option = DeviceOptionFlags::NONE,
            u32 adapter_id_override = UINT_MAX);

 public:
  /**
   * @brief ファクトリの取得
   */
  IDXGIFactory4* GetFactory() const { return factory_.Get(); }
  /**
   * @brief アダプターの取得k
   */
  IDXGIAdapter1* GetAdapter() const { return adapter_.Get(); }
  /**
   * @brief 現在有効なオプションの取得
   */
  DeviceOptionFlags GetOptions() const { return options_; }

 private:
  /**
   * @brief アダプターの初期化
   * @param adapter_id_override 上書きするアダプターID
   * @param adapter アダプター（戻り値）
   * @return 取得に成功したらtrueを返す
   */
  bool InitializeAdapter(u32 adapter_id_override, IDXGIAdapter1** adapter);

 private:
  //! ファクトリ
  ComPtr<IDXGIFactory6> factory_;
  //! アダプター
  ComPtr<IDXGIAdapter1> adapter_;
  //! デバイスオプション
  DeviceOptionFlags options_;
  //! アダプターID
  u32 adapter_id_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_DXGI_ADAPTER_H_
