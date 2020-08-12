#include "src/directx/device/dxgi_adapter.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace device {

using directx_helper::Failed;
using directx_helper::Succeeded;

//コンストラクタ
DXGIAdapter::DXGIAdapter() {}

//デストラクタ
DXGIAdapter::~DXGIAdapter() {}

//初期化
bool DXGIAdapter::Init(DeviceOptionFlags required_option,
                       u32 adapter_id_override) {
  this->options_ = required_option;

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
      if (Failed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG,
                                    IID_PPV_ARGS(&factory_)))) {
        return false;
      }
      debug_dxgi = true;
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
    if (Failed(CreateDXGIFactory1(IID_PPV_ARGS(&factory_)))) {
      return false;
    }
  }

  //ティアリング設定
  if (util::enum_util::IsBitpop(this->options_ & DeviceOptionFlags::TEARING)) {
    BOOL allow_tearing = false;
    ComPtr<IDXGIFactory5> factory_5;
    HRESULT hr = factory_.As(&factory_5);
    if (SUCCEEDED(hr)) {
      hr = factory_5->CheckFeatureSupport(
          DXGI_FEATURE::DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing,
          sizeof(allow_tearing));
    }
    if (FAILED(hr) || !allow_tearing) {
      MY_LOG(L"WARNING: Variable refresh rate displays are not supported.\n");
      this->options_ &= ~DeviceOptionFlags::TEARING;
    }
  }

  // WARPデバイスを作るかどうかで分岐する
  if (util::enum_util::IsBitpop(this->options_ &
                                DeviceOptionFlags::USE_WARP_DEVICE)) {
    if (Failed(factory_->EnumWarpAdapter(IID_PPV_ARGS(&adapter_)))) {
      return false;
    }
  } else {
    if (!InitializeAdapter(adapter_id_override, &adapter_)) {
      return false;
    }
  }

  return true;
}

//アダプターの初期化
bool DXGIAdapter::InitializeAdapter(u32 adapter_id_override,
                                    IDXGIAdapter1** adapter) {
  const DXGI_GPU_PREFERENCE pref =
      DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;

  *adapter = nullptr;
  ComPtr<IDXGIAdapter1> test_adapter;
  for (u32 adapter_id = 0; DXGI_ERROR_NOT_FOUND !=
                           factory_->EnumAdapterByGpuPreference(
                               adapter_id, pref, IID_PPV_ARGS(&test_adapter));
       adapter_id++) {
    //アダプターIDを上書きする設定ならそのIDと一致するまでループさせる
    if (adapter_id_override != UINT_MAX && adapter_id != adapter_id_override) {
      continue;
    }

    DXGI_ADAPTER_DESC1 desc;
    if (Failed(test_adapter->GetDesc1(&desc))) {
      return false;
    }
    if (desc.Flags & DXGI_ADAPTER_FLAG::DXGI_ADAPTER_FLAG_SOFTWARE) {
      continue;
    }

    //アダプターでデバイスを作成できればそのアダプターが有効である証となる
    if (Succeeded(D3D12CreateDevice(test_adapter.Get(),
                                    defines::MIN_FEATURE_LEVEL,
                                    __uuidof(ID3D12Device), nullptr))) {
      this->adapter_id_ = adapter_id;
      MY_LOG(L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls", adapter_id,
             desc.VendorId, desc.DeviceId, desc.Description);
      break;
    }
  }

  //ラップデバイスの設定
#if !defined(NDEBUG)
  if (!test_adapter && adapter_id_override == UINT_MAX) {
    if (Failed(factory_->EnumWarpAdapter(IID_PPV_ARGS(&test_adapter)))) {
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