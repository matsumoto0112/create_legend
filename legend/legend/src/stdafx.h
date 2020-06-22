#ifndef DX12_LIB_STDAFX_H_
#define DX12_LIB_STDAFX_H_

 #include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include <array>
#include <filesystem>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace dx12_lib {
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
}  // namespace dx12_lib

#endif  // !DX12_LIB_STDAFX_H_
