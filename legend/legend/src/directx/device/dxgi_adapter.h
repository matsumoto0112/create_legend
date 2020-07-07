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
class DXGIAdapter {
 public:
  DXGIAdapter();
  ~DXGIAdapter();
  bool Init(DeviceOptionFlags required_option = DeviceOptionFlags::NONE,
            u32 adapter_id_override = UINT_MAX);

 public:
  IDXGIFactory4* GetFactory() const { return factory_.Get(); }
  IDXGIAdapter1* GetAdapter() const { return adapter_.Get(); }
  DeviceOptionFlags GetOptions() const { return options_; }

 private:
  bool InitializeAdapter(IDXGIAdapter1** adapter, u32 adapter_id_override_);

 private:
  ComPtr<IDXGIFactory4> factory_;
  ComPtr<IDXGIAdapter1> adapter_;
  DeviceOptionFlags options_;
  u32 adapter_id_;
  std::wstring adapter_description_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DEVICE_DXGI_ADAPTER_H_
