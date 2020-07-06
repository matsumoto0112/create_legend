#include "src/directx/device/dxgi_adapter.h"

#include "src/directx/device/device_parameters.h"
#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace device {
DXGIAdapter::DXGIAdapter() {}
DXGIAdapter::~DXGIAdapter() {}
bool DXGIAdapter::Init(u32 options, u32 adapter_id_override) {
  this->options_ = options;

  if (this->options_ & parameters::REQUIRE_TEARING_SUPPORT) {
    this->options_ |= parameters::ALLOW_TEARING;
  }

  //デバッグ用DXGIオブジェクトを作成したかどうか
  bool debug_dxgi = false;
#ifdef _DEBUG
  {
    //デバッグ用の各種設定
    ComPtr<ID3D12Debug> debug_controller;
    if (HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller));
        SUCCEEDED(hr)) {
      debug_controller->EnableDebugLayer();
    } else {
      MY_LOG(L"WARNING: Direct3D Debug Device is not available\nReason : %s",
             directx_helper::HrToWString(hr).c_str());
    }

    ComPtr<IDXGIInfoQueue> dxgi_info_queue;
    if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgi_info_queue)))) {
      if (HRESULT hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG,
                                          IID_PPV_ARGS(&factory_));
          FAILED(hr)) {
        MY_LOG(L"CreateDXGIFactory2 failed.\n Reason : %s",
               directx_helper::HrToWString(hr));
      }
      dxgi_info_queue->SetBreakOnSeverity(
          DXGI_DEBUG_ALL,
          DXGI_INFO_QUEUE_MESSAGE_SEVERITY::
              DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR,
          true);
      dxgi_info_queue->SetBreakOnSeverity(
          DXGI_DEBUG_ALL,
          DXGI_INFO_QUEUE_MESSAGE_SEVERITY::
              DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION,
          true);
    }
  }
#endif
  //デバッグ用DXGIを作成していなければここで作成する
  if (!debug_dxgi) {
    if (HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory_)); FAILED(hr)) {
      MY_LOG(L"CreateDXGIFactory1 failed.\nReason : %s",
             directx_helper::HrToWString(hr));
    }
  }

  //ティアリング設定
  if (options_ &
      (parameters::ALLOW_TEARING | parameters::REQUIRE_TEARING_SUPPORT)) {
    bool allow_tearing = false;
    ComPtr<IDXGIFactory5> factory_5;
    HRESULT hr = factory_.As(&factory_5);
    if (SUCCEEDED(hr)) {
      hr = factory_5->CheckFeatureSupport(
          DXGI_FEATURE::DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing,
          sizeof(allow_tearing));
    }
    if (FAILED(hr) || !allow_tearing) {
      MY_LOG(L"WARNING: Variable refresh rate displays are not supported.\n");
      options_ &= ~parameters::ALLOW_TEARING;
    }
  }
  if (!InitializeAdapter(&adapter_, adapter_id_override)) {
    return false;
  }
  return true;
}
bool DXGIAdapter::InitializeAdapter(IDXGIAdapter1** adapter,
                                    u32 adapter_id_override) {
  *adapter = nullptr;
  ComPtr<IDXGIAdapter1> test_adapter;
  for (u32 adapter_id = 0; DXGI_ERROR_NOT_FOUND !=
                           factory_->EnumAdapters1(adapter_id, &test_adapter);
       adapter_id++) {
    //アダプターIDを上書きする設定ならそのIDと一致するまでループさせる
    if (adapter_id_override != UINT_MAX && adapter_id != adapter_id_override) {
      continue;
    }

    DXGI_ADAPTER_DESC1 desc;
    if (FAILED(test_adapter->GetDesc1(&desc))) {
      return false;
    }
    if (desc.Flags & DXGI_ADAPTER_FLAG::DXGI_ADAPTER_FLAG_SOFTWARE) {
      continue;
    }

    //アダプターでデバイスを作成できればそのアダプターが有効である証となる
    if (SUCCEEDED(D3D12CreateDevice(test_adapter.Get(),
                                    parameters::MIN_FEATURE_LEVEL,
                                    __uuidof(ID3D12Device), nullptr))) {
      this->adapter_id_ = adapter_id;
      this->adapter_description_ = desc.Description;
      MY_LOG(L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls", adapter_id,
             desc.VendorId, desc.DeviceId, desc.Description);
      break;
    }
  }

#ifdef _DEBUG
  if (!test_adapter && adapter_id_override == UINT_MAX) {
    if (FAILED(factory_->EnumWarpAdapter(IID_PPV_ARGS(&test_adapter)))) {
      MY_LOG(
          L"WARP12 not available. Enable the 'Graphics Tools' optional "
          L"feature");
      return false;
    }
    MY_LOG(L"Direct3D Adapter - WARP12");
  }
#endif

  //アダプターが作成できなかった
  if (!test_adapter) {
    if (adapter_id_override != UINT_MAX) {
      MY_LOG(L"Unavaiable adapter requested.");
      return false;
    } else {
      MY_LOG(L"Unavaiable adapter.");
      return false;
    }
  }

  *adapter = test_adapter.Detach();
  return true;
}
}  // namespace device
}  // namespace directx
}  // namespace legend